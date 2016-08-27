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

#include <QDateTimeEdit>
#include <QPixmap>
#include <QPushButton>
#include <QUdpSocket>
#include <QMainWindow>

#define WALLTIME_PANEL_USAGE "\n\n"

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

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void SendCommand(const QString &cmd);
  QDateTimeEdit *panel_preset_edit;
  QPushButton *panel_preset_button;
  QPushButton *panel_reset_button;
  QPushButton *panel_start_button;
  QPushButton *panel_stop_button;
  QUdpSocket *panel_socket;
};


#endif  // WALLTIME_PANEL_H
