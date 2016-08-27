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
#include <QHostAddress>

#include <QMessageBox>
#include <QStyleFactory>

#include "cmdswitch.h"
#include "profile.h"
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
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--clock-address") {
      if(!panel_clock_address.setAddress(cmd->value(i))) {
	QMessageBox::warning(this,"WallTime Panel",tr("Invalid IP Address!"));
	exit(256);
      }
    }
  }
  delete cmd;

  /*
  //
  // Create And Set Icon
  //
  setWindowIcon(QPixmap(lwpath_16x16_xpm));
  */

  //
  // Panel Configuration
  //
  if(!LoadConfig()) {
  }

  //
  // Fix the Window Size
  //
  //  setMinimumSize(sizeHint());

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
  panel_preset_button=new QPushButton(tr("Preset"),this);
  connect(panel_preset_button,SIGNAL(clicked()),this,SLOT(presetData()));

  panel_reset_button=new QPushButton(tr("Reset"),this);
  connect(panel_reset_button,SIGNAL(clicked()),this,SLOT(resetData()));

  panel_start_button=new QPushButton(tr("Start"),this);
  QObject::connect(panel_start_button,SIGNAL(clicked()),this,SLOT(startData()));

  panel_stop_button=new QPushButton(tr("Stop"),this);
  QObject::connect(panel_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  //
  // Verify Configuration
  //
  if(panel_clock_address.isNull()) {
    if(panel_config_dialog->exec(&panel_clock_address)) {
      SaveConfig();
    }
    else {
      exit(0);
    }
  }
}


QSize MainWidget::sizeHint() const
{
  return QSize(115,175);
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


void MainWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  int winc=w-20;
  int hinc=14*h/100;

  printf("%d X %d\n",w,h);

  QFont edit_font("helvetica",hinc/2,QFont::Normal);
  edit_font.setPixelSize(hinc/2);

  QFont button_font("helvetica",hinc/2,QFont::Bold);
  button_font.setPixelSize(hinc/2);

  panel_countmode_box->setGeometry(10,5,winc,hinc);

  panel_preset_edit->setGeometry(10,5+h/6,winc,hinc);
  panel_preset_edit->setFont(edit_font);


  panel_preset_button->setGeometry(10,5+2*h/6,winc,hinc);
  panel_preset_button->setFont(button_font);
  panel_reset_button->setGeometry(10,5+3*h/6,winc,hinc);
  panel_reset_button->setFont(button_font);
  panel_start_button->setGeometry(10,5+4*h/6,winc,hinc);
  panel_start_button->setFont(button_font);
  panel_stop_button->setGeometry(10,5+5*h/6,winc,hinc);
  panel_stop_button->setFont(button_font);
}


void MainWidget::SendCommand(const QString &cmd)
{
  panel_socket->
    writeDatagram(cmd.toUtf8(),cmd.length(),panel_clock_address,6060);
  printf("SendCommand(%s)\n",(const char *)cmd.toUtf8());
}


bool MainWidget::LoadConfig()
{
  bool ret=false;

  Profile *p=new Profile();
  if(p->setSource(ConfigFilename())) {
    panel_clock_address=p->addressValue("WalltimePanel","ClockAddress","");
  }
  delete p;

  return ret;
}


void MainWidget::SaveConfig()
{
  if(!ConfigFilename().isEmpty()) {
    FILE *f=NULL;
    QString tempfile=ConfigFilename()+"-temp";
    
    if((f=fopen(tempfile.toUtf8(),"w"))!=NULL) {
      fprintf(f,"[WalltimePanel]\n");
      fprintf(f,"ClockAddress=%s\n",
	      (const char *)panel_clock_address.toString().toUtf8());
      fprintf(f,"\n");
      fclose(f);
      rename(tempfile.toUtf8(),ConfigFilename().toUtf8());
    }
  }
}


QString MainWidget::ConfigFilename()
{
  QString ret;

  if(getenv("HOME")!=NULL) {
    ret=QString(getenv("HOME"))+"/.walltime-panel";
  }
  if(ret.isEmpty()) {
    QMessageBox::warning(this,"WallTime Panel - "+tr("Error"),
			 tr("Unable to determine configuration location!"));
  }

  return ret;
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
