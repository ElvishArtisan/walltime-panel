// configdialog.cpp
//
// Configuration dialog for Walltime-panel
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

#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle("WallTime Panel - "+tr("Configure"));
  setMinimumSize(sizeHint());

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  config_address_label=new QLabel(tr("Clock Address")+":",this);
  config_address_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  config_address_label->setFont(bold_font);

  config_keyaction_label=new QLabel(tr("SpaceBar Action")+":",this);
  config_keyaction_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  config_keyaction_label->setFont(bold_font);
  config_keyaction_box=new ComboBox(this);
  config_keyaction_box->insertItem(-1,tr("None"),ConfigDialog::None);
  config_keyaction_box->insertItem(-1,tr("Start"),ConfigDialog::Start);
  config_keyaction_box->insertItem(-1,tr("Stop"),ConfigDialog::Stop);
  config_keyaction_box->insertItem(-1,tr("Preset"),ConfigDialog::Preset);
  config_keyaction_box->insertItem(-1,tr("Clear"),ConfigDialog::Reset);

  config_ok_button=new QPushButton(tr("OK"),this);
  config_ok_button->setFont(bold_font);
  connect(config_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  config_cancel_button=new QPushButton(tr("Cancel"),this);
  config_cancel_button->setFont(bold_font);
  connect(config_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  config_address_edit=new QLineEdit(this);
}


QSize ConfigDialog::sizeHint() const
{
  return QSize(325,115);
}


int ConfigDialog::exec(QHostAddress *clock_addr,
		       ConfigDialog::KeyAction *key_action)
{
  config_clock_address=clock_addr;
  config_key_action=key_action;
  if(!config_clock_address->isNull()) {
    config_address_edit->setText(config_clock_address->toString());
  }
  config_key_action=key_action;
  config_keyaction_box->setCurrentItemData(*key_action);

  return QDialog::exec();
}


void ConfigDialog::okData()
{
  if(!config_clock_address->setAddress(config_address_edit->text())) {
    QMessageBox::warning(this,"WallTime Panel - "+tr("Error"),
			 tr("Invalid IP Address!"));
    return;
  }
  *config_key_action=
    (ConfigDialog::KeyAction)config_keyaction_box->currentItemData().toInt();
  done(true);
}


void ConfigDialog::cancelData()
{
  done(false);
}


void ConfigDialog::resizeEvent(QResizeEvent *e)
{
  config_address_label->setGeometry(10,10,135,20);
  config_address_edit->setGeometry(150,10,size().width()-145,20);

  config_keyaction_label->setGeometry(10,40,135,20);
  config_keyaction_box->setGeometry(150,40,100,20);

  config_ok_button->setGeometry(size().width()-140,size().height()-40,60,30);

  config_cancel_button->setGeometry(size().width()-70,size().height()-40,60,30);
}
