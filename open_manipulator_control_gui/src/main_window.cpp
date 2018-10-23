﻿/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/open_manipulator_control_gui/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace open_manipulator_control_gui {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
  : QMainWindow(parent)
  , qnode(argc,argv)
{
  ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
  QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application
  connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

  qnode.init();

}

MainWindow::~MainWindow() {}

void MainWindow::timerCallback()
{
  std::vector<double> joint_angle = qnode.getPresentJointAngle();
  ui.txt_j1->setText(QString::number(joint_angle.at(0),'f', 3));
  ui.txt_j2->setText(QString::number(joint_angle.at(1),'f', 3));
  ui.txt_j3->setText(QString::number(joint_angle.at(2),'f', 3));
  ui.txt_j4->setText(QString::number(joint_angle.at(3),'f', 3));

  std::vector<double> position = qnode.getPresentKinematicsPose();
  ui.txt_x->setText(QString::number(position.at(0),'f', 3));
  ui.txt_y->setText(QString::number(position.at(1),'f', 3));
  ui.txt_z->setText(QString::number(position.at(2),'f', 3));
}
void MainWindow::tabSelected()
{
  if(ui.tabWidget->currentIndex()==0)
    on_btn_read_joint_angle_clicked();
  if(ui.tabWidget->currentIndex()==1)
    on_btn_read_kinematic_pose_clicked();
}

void MainWindow::writeLog(QString str)
{
  ui.plainTextEdit_log->moveCursor (QTextCursor::End);
  ui.plainTextEdit_log->appendPlainText(str);
}

void MainWindow::on_btn_timer_start_clicked(void)
{
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(timerCallback()));
  timer->start(100);

  writeLog("QTimer start : 100ms");
  ui.btn_timer_start->setEnabled(false);
  ui.btn_gripper_off->setEnabled(true);
  ui.btn_gripper_on->setEnabled(true);
  ui.btn_home_pose->setEnabled(true);
  ui.btn_init_pose->setEnabled(true);
  ui.btn_read_joint_angle->setEnabled(true);
  ui.btn_read_kinematic_pose->setEnabled(true);
  ui.btn_send_joint_angle->setEnabled(true);
  ui.btn_send_kinematic_pose->setEnabled(true);
}

void MainWindow::on_btn_init_pose_clicked(void)
{
  std::vector<std::string> joint_name;
  std::vector<double> joint_angle;
  double path_time = 2.0;
  joint_name.push_back("joint1"); joint_angle.push_back(0.0);
  joint_name.push_back("joint2"); joint_angle.push_back(0.0);
  joint_name.push_back("joint3"); joint_angle.push_back(0.0);
  joint_name.push_back("joint4"); joint_angle.push_back(0.0);

  qnode.setJointSpacePath(joint_name, joint_angle, path_time);

  writeLog("Send joint angle to init. pose.");
}

void MainWindow::on_btn_home_pose_clicked(void)
{
  std::vector<std::string> joint_name;
  std::vector<double> joint_angle;
  double path_time = 2.0;

  joint_name.push_back("joint1"); joint_angle.push_back(0.0);
  joint_name.push_back("joint2"); joint_angle.push_back(-1.05);
  joint_name.push_back("joint3"); joint_angle.push_back(0.35);
  joint_name.push_back("joint4"); joint_angle.push_back(0.70);

  qnode.setJointSpacePath(joint_name, joint_angle, path_time);

  writeLog("Send joint angle to home pose.");
}

void MainWindow::on_btn_gripper_off_clicked(void)
{
  std::vector<double> joint_angle;
  joint_angle.push_back(-0.01);
  qnode.setToolControl(joint_angle);

  writeLog("Send gripper off");
}

void MainWindow::on_btn_gripper_on_clicked(void)
{
  std::vector<double> joint_angle;
  joint_angle.push_back(0.015);
  qnode.setToolControl(joint_angle);

  writeLog("Send gripper on");
}


void MainWindow::on_btn_read_joint_angle_clicked(void)
{
  std::vector<double> joint_angle = qnode.getPresentJointAngle();
  ui.doubleSpinBox_j1->setValue(joint_angle.at(0));
  ui.doubleSpinBox_j2->setValue(joint_angle.at(1));
  ui.doubleSpinBox_j3->setValue(joint_angle.at(2));
  ui.doubleSpinBox_j4->setValue(joint_angle.at(3));
}
void MainWindow::on_btn_send_joint_angle_clicked(void)
{
  std::vector<std::string> joint_name;
  std::vector<double> joint_angle;
  double path_time = ui.doubleSpinBox_time_js->value();

  joint_name.push_back("joint1"); joint_angle.push_back(ui.doubleSpinBox_j1->value());
  joint_name.push_back("joint2"); joint_angle.push_back(ui.doubleSpinBox_j2->value());
  joint_name.push_back("joint3"); joint_angle.push_back(ui.doubleSpinBox_j3->value());
  joint_name.push_back("joint4"); joint_angle.push_back(ui.doubleSpinBox_j4->value());

  qnode.setJointSpacePath(joint_name, joint_angle, path_time);

  writeLog("Send joint angle to home pose.");
}
void MainWindow::on_btn_read_kinematic_pose_clicked(void)
{
  std::vector<double> position = qnode.getPresentKinematicsPose();
  ui.doubleSpinBox_x->setValue(position.at(0));
  ui.doubleSpinBox_y->setValue(position.at(1));
  ui.doubleSpinBox_z->setValue(position.at(2));
}
void MainWindow::on_btn_send_kinematic_pose_clicked(void)
{
  std::vector<double> kinematics_pose;
  double path_time = ui.doubleSpinBox_time_cs->value();

  kinematics_pose.push_back(ui.doubleSpinBox_x->value());
  kinematics_pose.push_back(ui.doubleSpinBox_y->value());
  kinematics_pose.push_back(ui.doubleSpinBox_z->value());

  qnode.setTaskSpacePath(kinematics_pose, path_time);

  writeLog("Send joint angle to home pose.");
}


}  // namespace open_manipulator_control_gui

