/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "ShadowedBox.h"
#include "BoxShadowNode.h"

#include <QskBoxNode.h>
#include <QskBoxBorderMetrics.h>
#include <QskBoxBorderColors.h>
#include <QskGradient.h>
#include <QskSkinlet.h>

namespace
{
    class Skinlet : public QskSkinlet
    {
      public:
        enum NodeRole { ShadowRole, PanelRole };

        Skinlet()
        {
            setNodeRoles( { ShadowRole, PanelRole } );
        }

        QRectF subControlRect( const QskSkinnable*,
            const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const override
        {
            if ( subControl == ShadowedBox::Panel )
            {
                return contentsRect;
            }

            return QRectF();
        }

        QSGNode* updateSubNode( const QskSkinnable* skinnable,
            quint8 nodeRole, QSGNode* node ) const override
        {
            const auto box = static_cast< const ShadowedBox* >( skinnable );

            switch ( nodeRole )
            {
                case ShadowRole:
                {
                    auto shadowNode = static_cast< BoxShadowNode* >( node );
                    if ( shadowNode == nullptr )
                        shadowNode = new BoxShadowNode();

                    const auto& s = box->shadow();
                    const qreal dx = s.extent + s.xOffset;
                    const qreal dy = s.extent + s.yOffset;

                    auto r = box->subControlRect( ShadowedBox::Panel );
                    r.adjust( -dx, -dy, dx, dy );

                    shadowNode->setRect( r );
                    shadowNode->setShape( box->shape() );
                    shadowNode->setShadow( s.extent, s.xOffset, s.yOffset );
                    shadowNode->setColor( box->shadowColor() );
                    shadowNode->updateGeometry();

                    return shadowNode;
                }
                case PanelRole:
                {
                    auto boxNode = static_cast< QskBoxNode* >( node );
                    if ( boxNode == nullptr )
                        boxNode = new QskBoxNode();

                    const QRectF r = box->subControlRect( ShadowedBox::Panel );

                    boxNode->setBoxData( r, box->shape(), QskBoxBorderMetrics(),
                        QskBoxBorderColors(), box->gradient() );

                    return boxNode;
                }
            }

            return nullptr;
        }
    };
}

QSK_SUBCONTROL( ShadowedBox, Panel )

ShadowedBox::ShadowedBox( QQuickItem* parentItem )
    : QskControl( parentItem )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setSkinlet( new Skinlet() );
}

ShadowedBox::~ShadowedBox()
{
}

void ShadowedBox::setShadow( const Shadow& shadow )
{
    m_shadow = shadow;
    update();
}

const ShadowedBox::Shadow& ShadowedBox::shadow() const
{
    return m_shadow;
}

void ShadowedBox::setShadowColor( const QColor& color )
{
    m_shadowColor = color;
    update();
}

QColor ShadowedBox::shadowColor() const
{
    return m_shadowColor;
}

void ShadowedBox::setGradient( const QskGradient& gradient )
{
    m_gradient = gradient;
    update();
}

const QskGradient& ShadowedBox::gradient() const
{
    return m_gradient;
}

void ShadowedBox::setShape( const QskBoxShapeMetrics& shape )
{
    m_shape = shape;
    update();
}

const QskBoxShapeMetrics& ShadowedBox::shape() const
{
    return m_shape;
}

#include "moc_ShadowedBox.cpp"
