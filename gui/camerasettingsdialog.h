#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "../hardware/camera/camera.h"

#include "../hardware/scanmanager.h"
#include "../hardware/hardwaremanager.h"

namespace Ui {
class CameraSettingsDialog;
}
/**
  Dialog for adding cameras to the setup.
  */
class CameraSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CameraSettingsDialog(QWidget *parent = 0);
    /**
      Set the ScanManager that this dialog governs.
      */
    void setHardwareManager(HardwareManager *m){hardware = m;}
    void setScanManager(ScanManager *m){manager = m;}
    ~CameraSettingsDialog();

public slots:
    
private:
    Ui::CameraSettingsDialog *ui;

    /// hardware manager
    HardwareManager *hardware;
    /// scan manager
    ScanManager *manager;

};

#endif // CAMERASETTINGSDIALOG_H
