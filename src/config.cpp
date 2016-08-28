// config.cpp
//
// Configuration Methods
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

#include <QMessageBox>

#include "profile.h"
#include "walltime-panel.h"

bool MainWidget::LoadConfig()
{
  bool ret=false;

  Profile *p=new Profile();
  if(p->setSource(ConfigFilename())) {
    panel_clock_address=p->addressValue("WalltimePanel","ClockAddress","");
    panel_countmode_box->
      setCurrentItemData(p->stringValue("WalltimePanel","CountMode","SM U!"));
    panel_preset_edit->
      setTime(QTime().addSecs(p->intValue("WalltimePanel","Preset")));
    int x=p->intValue("WalltimePanel","GeometryX",geometry().x());
    int y=p->intValue("WalltimePanel","GeometryY",geometry().y());
    int w=p->intValue("WalltimePanel","GeometryWidth",sizeHint().width());
    int h=p->intValue("WalltimePanel","GeometryHeight",sizeHint().height());
    setGeometry(x,y,w,h);
  }
  else {
    setGeometry(geometry().x(),geometry().y(),
		sizeHint().width(),sizeHint().height());
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
      fprintf(f,"CountMode=%s\n",(const char *)
	      panel_countmode_box->currentItemData().toString().toUtf8());
      fprintf(f,"Preset=%d\n",QTime().secsTo(panel_preset_edit->time()));
      fprintf(f,"GeometryX=%d\n",geometry().x());
      fprintf(f,"GeometryY=%d\n",geometry().y());
      fprintf(f,"GeometryWidth=%d\n",geometry().width());
      fprintf(f,"GeometryHeight=%d\n",geometry().height());
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
