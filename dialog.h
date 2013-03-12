#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class QTableWidget;
class HttpHandle;
class InformationAnalyze;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    ~Dialog();
private slots:
    void OnBtnDownloadClicked();
    void slt_vRefresh();
public slots:
    void slt_vUpdate();

private:
    HttpHandle *m_pHttpHandle;
    InformationAnalyze *m_pInformationAnalyze;
    QTableWidget *m_pTableWidget;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
