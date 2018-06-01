/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_INPUT_PANEL_H
#define QSK_INPUT_PANEL_H

#include "QskGlobal.h"
#include "QskBox.h"

class QskInputEngine;

class QString;
class QLocale;

class QSK_EXPORT QskInputPanel : public QskBox
{
    Q_OBJECT

    using Inherited = QskBox;

    Q_PROPERTY( bool inputProxy READ hasInputProxy
        WRITE setInputProxy NOTIFY inputProxyChanged )

    Q_PROPERTY( QString inputPrompt READ inputPrompt
        WRITE setInputPrompt NOTIFY inputPromptChanged )

public:
    QSK_SUBCONTROLS( Panel )

    QskInputPanel( QQuickItem* parent = nullptr );
    virtual ~QskInputPanel() override;

    void attachInputItem( QQuickItem* );
    QQuickItem* attachedInputItem() const;

    void setEngine( QskInputEngine* );
    QskInputEngine* engine();

    bool hasInputProxy() const;
    QQuickItem* inputProxy() const;

    QString inputPrompt() const;

    virtual QskAspect::Subcontrol effectiveSubcontrol(
        QskAspect::Subcontrol ) const override;

    virtual void processInputMethodQueries( Qt::InputMethodQueries );

Q_SIGNALS:
    void inputProxyChanged( bool );
    void inputPromptChanged( const QString& );

    void textEntered( const QString&, bool isFinal );
    void keyEntered( int keyCode );
    void done( bool success );

public Q_SLOTS:
    void setInputPrompt( const QString& );
    void setInputProxy( bool );

protected:
    virtual void keyPressEvent( QKeyEvent* ) override;
    virtual void keyReleaseEvent( QKeyEvent* ) override;

    virtual void processKey( int key,
        Qt::InputMethodHints, int spaceLeft );

    virtual void updatePrediction();

private:
    void commitKey( int key );
    void commitPredictiveText( int );

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif
