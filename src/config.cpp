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

#include <QSettings>

#include "configdialog.h"
#include "walltime-panel.h"

void MainWidget::LoadConfig()
{
  QSettings s;

  panel_clock_address.setAddress(s.value("ClockAddress").toString());
  panel_countmode_box->
    setCurrentItemData(s.value("CountMode","SM U!").toString());
  panel_preset_edit->setTime(QTime().addSecs(s.value("Preset").toInt()));
  panel_key_action=(ConfigDialog::KeyAction)
    s.value("SpacebarAction",(int)ConfigDialog::None).toInt();
  resize(s.value("MainWindowSize",sizeHint()).toSize());
  move(s.value("MainWindowPosition",
	       QPoint(geometry().x(),geometry().y())).toPoint());
}


void MainWidget::SaveConfig()
{
  QSettings s;

  s.setValue("ClockAddress",panel_clock_address.toString());
  s.setValue("CountMode",panel_countmode_box->currentItemData().toString());
  s.setValue("Preset",QTime().secsTo(panel_preset_edit->time()));
  s.setValue("SpacebarAction",(int)panel_key_action);
  s.setValue("MainWindowSize",size());
  s.setValue("MainWindowPosition",pos());
}
