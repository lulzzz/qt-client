/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2015 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __GUIMESSAGEHANDLER_H__
#define __GUIMESSAGEHANDLER_H__

#include "xabstractmessagehandler.h"

class GuiMessageHandlerPrivate;

class GuiMessageHandler : public XAbstractMessageHandler
{
  public:
    GuiMessageHandler(QObject *parent = 0);
    virtual ~GuiMessageHandler();

    virtual bool setDestination(QtMsgType type, QWidget *destination);
    virtual QMessageBox::StandardButton question(const QString &question, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

  protected:
    virtual void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

    GuiMessageHandlerPrivate *_p;
};

#endif
