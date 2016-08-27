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

#include <QApplication>
#include <QHostAddress>

#include <QStyleFactory>

#include "cmdswitch.h"
#include "walltime-panel.h"

//
// Icons
//
//#include "../../icons/lwpath-16x16.xpm"


MainWidget::MainWidget(QWidget *parent)
  :QMainWindow(parent)
{
  //
  // Read Command Options
  //
  CmdSwitch *cmd=new CmdSwitch("walltime-panel",VERSION,WALLTIME_PANEL_USAGE);
  delete cmd;

  /*
  //
  // Create And Set Icon
  //
  setWindowIcon(QPixmap(lwpath_16x16_xpm));

  //
  // Panel Configuration
  //
  config=new Config();
  if(!config->load(conffile)) {
    QMessageBox::warning(this,"LWPanel - "+tr("Error"),
			 tr("Unable to open configuration file at")+
			 " \""+conffile+"\"!");
    exit(256);
  }
  */
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  //
  // UDP Socket
  //
  panel_socket=new QUdpSocket(this);

  //
  // Preset Counter
  //
  panel_preset_edit=new QDateTimeEdit(this);
  panel_preset_edit->setDisplayFormat("hh:mm:ss");

  //
  // Buttons
  //
  panel_preset_button=new QPushButton(tr("Preset"),this);
  connect(panel_preset_button,SIGNAL(clicked()),this,SLOT(presetData()));

  panel_reset_button=new QPushButton(tr("Reset"),this);
  connect(panel_reset_button,SIGNAL(clicked()),this,SLOT(resetData()));

  panel_start_button=new QPushButton(tr("Start"),this);
  QObject::connect(panel_start_button,SIGNAL(clicked()),this,SLOT(startData()));

  panel_stop_button=new QPushButton(tr("Stop"),this);
  QObject::connect(panel_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));
}


QSize MainWidget::sizeHint() const
{
  return QSize(150,250);
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
  SendCommand("ST!");
}


void MainWidget::stopData()
{
  SendCommand("SP!");
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  int winc=w-20;
  int hinc=18*h/100;

  QFont edit_font("helvetica",hinc/2,QFont::Normal);
  edit_font.setPixelSize(hinc/2);

  panel_preset_edit->setGeometry(10,5,winc,hinc);
  panel_preset_edit->setFont(edit_font);

  QFont button_font("helvetica",hinc/2,QFont::Bold);
  button_font.setPixelSize(hinc/2);

  panel_preset_button->setGeometry(10,5+h/5,winc,hinc);
  panel_preset_button->setFont(button_font);
  panel_reset_button->setGeometry(10,5+2*h/5,winc,hinc);
  panel_reset_button->setFont(button_font);
  panel_start_button->setGeometry(10,5+3*h/5,winc,hinc);
  panel_start_button->setFont(button_font);
  panel_stop_button->setGeometry(10,5+4*h/5,winc,hinc);
  panel_stop_button->setFont(button_font);
}


void MainWidget::SendCommand(const QString &cmd)
{
  panel_socket->writeDatagram(cmd.toUtf8(),cmd.length(),
			      QHostAddress("192.168.21.100"),6060);
  printf("SendCommand(%s)\n",(const char *)cmd.toUtf8());
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  a.setStyle(QStyleFactory::create("Windows"));

  //
  // Start Event Loop
  //
  MainWidget *w=new MainWidget(NULL);
  //  a.setMainWidget(w);
  w->setGeometry(w->geometry().x(),w->geometry().y(),w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}
