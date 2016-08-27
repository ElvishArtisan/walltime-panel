// configdialog.h
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ConfigDialog : public QDialog
{
  Q_OBJECT
 public:
  ConfigDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(QHostAddress *clock_addr);

 private slots:
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *config_address_label;
  QLineEdit *config_address_edit;
  QPushButton *config_ok_button;
  QPushButton *config_cancel_button;
  QHostAddress *config_clock_address;
};


#endif  // CONFIGDIALOG_H
