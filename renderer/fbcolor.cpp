#include "fbcolor.h"
#include "layernode.h"

void FgBgColorComponent::useLayerBgColor ( LayerNode* player )
{
    setUseLayerBgColor ( true );
    FgBgColor::setBgColorPtr ( player->getColorPtr() );
}
void FgBgColorComponent::setBgColor ( GColor color )
{
    setUseLayerBgColor ( false );
    FgBgColor::setBgColor ( color );
}
GColor FgBgColorComponent::getBgColor () const
{
    if ( isUseLayerBgColor() )
        return *(FgBgColor::getBgColorPtr ());
    else
        return FgBgColor::getBgColor ();
}

void FgBgColorComponent::useLayerFgColor ( LayerNode* player )
{
    setUseLayerFgColor ( true );
    FgBgColor::setFgColorPtr ( player->getColorPtr() );
}

void FgBgColorComponent::setFgColor ( GColor color )
{
    setUseLayerFgColor ( false );
    FgBgColor::setFgColor ( color );
}
GColor FgBgColorComponent::getFgColor ()
{
    if ( isUseLayerFgColor() )
    {
        GColor* p=FgBgColor::getFgColorPtr ();
        return *p;
    }
    else
        return FgBgColor::getFgColor ();
}

