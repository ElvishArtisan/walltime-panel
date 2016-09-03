// walltime-panel.cpp
//
// Segment Timer Control Applet for WallTime
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdio.h>

#include <QApplication>
#include <QCoreApplication>
#include <QFontDatabase>
#include <QFontInfo>
#include <QFontMetrics>
#include <QHostAddress>
#include <QList>
#include <QMessageBox>

#include <QStyleFactory>

#include "cmdswitch.h"
#include "walltime-panel.h"

//
// Icons
//
#include "../icons/settings.xpm"
#include "../icons/walltime_logo.xpm"
#include "../icons/walltime-16x16.xpm"


MainWidget::MainWidget(QWidget *parent)
  :QMainWindow(parent)
{
  //
  // Read Command Options
  //
  CmdSwitch *cmd=new CmdSwitch("walltime-panel",VERSION,WALLTIME_PANEL_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--clock-address") {
      if(!panel_clock_address.setAddress(cmd->value(i))) {
	QMessageBox::warning(this,"WallTime Panel",tr("Invalid IP Address!"));
	exit(256);
      }
    }
  }
  delete cmd;

  QCoreApplication::setOrganizationName("Paravel Systems");
  QCoreApplication::setOrganizationDomain("paravelsystems.com");
  QCoreApplication::setApplicationName("walltime-panel");

  //
  // Window Title Bar
  //
  setWindowTitle(tr("WallTime Remote Control"));
  setWindowIcon(QPixmap(walltime_16x16_xpm));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // UDP Socket
  //
  panel_socket=new QUdpSocket(this);

  //
  // Configuration Dialog
  //
  panel_config_dialog=new ConfigDialog(this);


  //
  // Direction
  //
  panel_countmode_box=new ComboBox(this);
  panel_countmode_box->setFocusPolicy(Qt::NoFocus);
  panel_countmode_box->insertItem(0,tr("Count Up"),"SM U!");
  panel_countmode_box->insertItem(1,tr("Count Down"),"SM D!");

  //
  // Preset Counter
  //
  panel_preset_edit=new QDateTimeEdit(this);
  panel_preset_edit->setDisplayFormat("hh:mm:ss");

  //
  // Buttons
  //
  panel_preset_button=new QPushButton(tr("&Preset"),this);
  panel_preset_button->setFocusPolicy(Qt::NoFocus);
  connect(panel_preset_button,SIGNAL(clicked()),this,SLOT(presetData()));

  panel_reset_button=new QPushButton(tr("&Clear"),this);
  panel_reset_button->setFocusPolicy(Qt::NoFocus);
  connect(panel_reset_button,SIGNAL(clicked()),this,SLOT(resetData()));

  panel_start_button=new QPushButton(tr("&Start"),this);
  panel_start_button->setFocusPolicy(Qt::NoFocus);
  connect(panel_start_button,SIGNAL(clicked()),this,SLOT(startData()));

  panel_stop_button=new QPushButton(tr("S&top"),this);
  panel_stop_button->setFocusPolicy(Qt::NoFocus);
  connect(panel_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  panel_logo_label=new QLabel(this);
  panel_logo_label->setPixmap(QPixmap(walltime_logo_xpm));
  panel_logo_label->setScaledContents(true);

  panel_config_button=new QPushButton(this);
  panel_config_button->setIcon(QPixmap(settings_xpm));
  panel_config_button->setFocusPolicy(Qt::NoFocus);
  connect(panel_config_button,SIGNAL(clicked()),this,SLOT(configData()));

  MakeFontMap();

  //
  // Verify Configuration
  //
  LoadConfig();
  if(panel_clock_address.isNull()) {
    if(panel_config_dialog->exec(&panel_clock_address,&panel_key_action)) {
      SaveConfig();
    }
    else {
      exit(0);
    }
  }
}


QSize MainWidget::sizeHint() const
{
  return QSize(150,200);
}


void MainWidget::presetData()
{
  SendCommand(QString().sprintf("PS %d!",
				QTime().secsTo(panel_preset_edit->time())));
}


void MainWidget::resetData()
{
  SendCommand("RS!");
}


void MainWidget::startData()
{
  SendCommand(panel_countmode_box->currentItemData().toString()+"ST!");
}


void MainWidget::stopData()
{
  SendCommand("SP!");
}


void MainWidget::configData()
{
  if(panel_config_dialog->exec(&panel_clock_address,&panel_key_action)) {
    SaveConfig();
  }
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  SaveConfig();
  e->accept();
}


void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
  switch(e->key()) {
  case Qt::Key_Space:
    switch(panel_key_action) {
    case ConfigDialog::Start:
      startData();
      break;

    case ConfigDialog::Stop:
      stopData();
      break;

    case ConfigDialog::Preset:
      presetData();
      break;

    case ConfigDialog::Reset:
      resetData();
      break;

    case ConfigDialog::None:
      break;
    }
    e->accept();
    break;

  case Qt::Key_S:
    startData();
    e->accept();
    break;

  case Qt::Key_T:
    stopData();
    e->accept();
    break;

  case Qt::Key_P:
    presetData();
    e->accept();
    break;

  case Qt::Key_C:
    resetData();
    e->accept();
    break;
  }
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  int winc=w-20;
  int hinc=12*h/100;

  //  printf("%d X %d\n",w,h);


  //
  // Calculate Fonts
  //
  QFont edit_font=font();
  for(int i=(int)panel_font_sizes.size()-1;i>=0;i--) {
    if((panel_countdown_font_widths[i]<4*winc/5)&&(panel_font_sizes[i]<hinc/2)) {
      edit_font=QFont(font().family(),panel_font_sizes[i],QFont::Normal);
      break;
    }
  }
  QFont start_font("helvetica",hinc/2,QFont::Bold);
  for(int i=(int)panel_font_sizes.size()-1;i>=0;i--) {
    if((panel_start_font_widths[i]<(3*winc/5))&&(panel_font_sizes[i]<7*hinc/10)) {
      start_font=QFont(font().family(),panel_font_sizes[i],QFont::Bold);
      break;
    }
  }
  QFont preset_font(font().family(),hinc/2,QFont::Bold);
  for(int i=(int)panel_font_sizes.size()-1;i>=0;i--) {
    if((panel_preset_font_widths[i]<(winc/3))&&(panel_font_sizes[i]<hinc/2)) {
      preset_font=QFont(font().family(),panel_font_sizes[i],QFont::Bold);
      break;
    }
  }

  //
  // Calculate Logo Size
  //
  int logo_width=500*hinc/187;
  int logo_height=hinc;
  if(logo_width>(w-20-hinc)) {
    logo_width=w-20-hinc;
    logo_height=187*logo_width/500;
  }

  panel_countmode_box->setGeometry(10,5,winc,hinc);
  panel_countmode_box->setFont(edit_font);

  panel_preset_edit->setGeometry(10,5+1*h/7,winc,hinc);
  panel_preset_edit->setFont(edit_font);

  panel_preset_button->setGeometry(10,5+2*h/7,winc/2-10+6,hinc);
  panel_preset_button->setFont(preset_font);

  panel_reset_button->setGeometry(13+winc/2,5+2*h/7,winc/2-10+6,hinc);
  panel_reset_button->setFont(preset_font);

  panel_start_button->setGeometry(10,5+3*h/7,winc,3*hinc/2);
  panel_start_button->setFont(start_font);

  panel_stop_button->setGeometry(10,5+9*h/14,winc,3*hinc/2);
  panel_stop_button->setFont(start_font);

  panel_logo_label->setGeometry(10,5+6*h/7,logo_width,logo_height);

  panel_config_button->setGeometry(w-10-hinc,5+6*h/7,hinc,hinc);
}


void MainWidget::MakeFontMap()
{
  QList<int> sizes;
  QFontDatabase fdb;

  QFontInfo info(font());
  sizes=fdb.smoothSizes(info.family(),"Bold");
  if(sizes.size()==0) {
    sizes=fdb.pointSizes(info.family());
  }
  for(int i=0;i<sizes.size();i++) {
    QFont font(info.family(),sizes[i],QFont::Bold);
    QFontMetrics fm(font);
    panel_font_sizes.push_back(sizes[i]);
    panel_countdown_font_widths.push_back(fm.width(tr("Count Down")));
    panel_preset_font_widths.push_back(fm.width(tr("Preset")));
    panel_start_font_widths.push_back(fm.width(tr("Start")));
  }
}


void MainWidget::SendCommand(const QString &cmd)
{
  panel_socket->
    writeDatagram(cmd.toUtf8(),cmd.length(),panel_clock_address,6060);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  a.setStyle(QStyleFactory::create("Windows"));

  //
  // Start Event Loop
  //
  MainWidget *w=new MainWidget();
  w->show();
  return a.exec();
}
