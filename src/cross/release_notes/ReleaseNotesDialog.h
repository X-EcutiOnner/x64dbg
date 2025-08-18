#pragma once

#include <QDialog>

namespace Ui
{
    class ReleaseNotesDialog;
}

class ReleaseNotesDialog : public QDialog
{
    Q_OBJECT

public:
    ReleaseNotesDialog(const QByteArray & markdown, QWidget* parent = nullptr);
    ~ReleaseNotesDialog();

private:
    Ui::ReleaseNotesDialog* ui;
};
