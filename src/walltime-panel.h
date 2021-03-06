// walltime-panel.h
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

#ifndef WALLTIME_PANEL_H
#define WALLTIME_PANEL_H

#include <vector>

#include <QDateTimeEdit>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QUdpSocket>
#include <QMainWindow>

#include "combobox.h"
#include "configdialog.h"

#define WALLTIME_PANEL_USAGE "--clock-address=<ip-addr>\n\n"

class MainWidget : public QMainWindow
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void presetData();
  void resetData();
  void startData();
  void stopData();
  void configData();

 protected:
  void closeEvent(QCloseEvent *e);
  void keyReleaseEvent(QKeyEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void SendCommand(const QString &cmd);
  void MakeFontMap();
  void LoadConfig();
  void SaveConfig();
  ComboBox *panel_countmode_box;
  QDateTimeEdit *panel_preset_edit;
  QPushButton *panel_preset_button;
  QPushButton *panel_reset_button;
  QPushButton *panel_start_button;
  QPushButton *panel_stop_button;
  QLabel *panel_logo_label;
  QPushButton *panel_config_button;
  ConfigDialog *panel_config_dialog;
  QUdpSocket *panel_socket;
  QHostAddress panel_clock_address;
  ConfigDialog::KeyAction panel_key_action;
  std::vector<int> panel_font_sizes;
  std::vector<int> panel_countdown_font_widths;
  std::vector<int> panel_preset_font_widths;
  std::vector<int> panel_start_font_widths;
};


#endif  // WALLTIME_PANEL_H
