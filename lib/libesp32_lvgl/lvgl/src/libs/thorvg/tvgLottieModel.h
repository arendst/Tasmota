/*
 * Copyright (c) 2023 - 2024 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../../lv_conf_internal.h"
#if LV_USE_THORVG_INTERNAL

#ifndef _TVG_LOTTIE_MODEL_H_
#define _TVG_LOTTIE_MODEL_H_

#include <cstring>

#include "tvgCommon.h"
#include "tvgRender.h"
#include "tvgLottieProperty.h"


struct LottieComposition;

struct LottieStroke
{
    struct DashAttr
    {
        //0: offset, 1: dash, 2: gap
        LottieFloat value[3] = {0.0f, 0.0f, 0.0f};
    };

    virtual ~LottieStroke()
    {
        delete(dashattr);
    }

    LottieFloat& dash(int no)
    {
        if (!dashattr) dashattr = new DashAttr;
        return dashattr->value[no];
    }

    float dashOffset(float frameNo, LottieExpressions* exps)
    {
        return dash(0)(frameNo, exps);
    }

    float dashGap(float frameNo, LottieExpressions* exps)
    {
        return dash(2)(frameNo, exps);
    }

    float dashSize(float frameNo, LottieExpressions* exps)
    {
        auto d = dash(1)(frameNo, exps);
        if (d == 0.0f) return 0.1f;
        else return d;
    }

    LottieFloat width = 0.0f;
    DashAttr* dashattr = nullptr;
    float miterLimit = 0;
    StrokeCap cap = StrokeCap::Round;
    StrokeJoin join = StrokeJoin::Round;
};


struct LottieMask
{
    LottiePathSet pathset;
    LottieOpacity opacity = 255;
    CompositeMethod method;
    bool inverse = false;
};


struct LottieObject
{
    enum Type : uint8_t
    {
        Composition = 0,
        Layer,
        Group,
        Transform,
        SolidFill,
        SolidStroke,
        GradientFill,
        GradientStroke,
        Rect,
        Ellipse,
        Path,
        Polystar,
        Image,
        Trimpath,
        Text,
        Repeater,
        RoundedCorner
    };

    virtual ~LottieObject()
    {
        free(name);
    }

    virtual void override(LottieProperty* prop)
    {
        TVGERR("LOTTIE", "Unsupported slot type");
    }

    virtual bool mergeable() { return false; }

    char* name = nullptr;
    Type type;
    bool hidden = false;       //remove?
};


struct LottieGlyph
{
    Array<LottieObject*> children;   //glyph shapes.
    float width;
    char* code;
    char* family = nullptr;
    char* style = nullptr;
    uint16_t size;
    uint8_t len;

    void prepare()
    {
        len = strlen(code);
    }

    ~LottieGlyph()
    {
        for (auto p = children.begin(); p < children.end(); ++p) delete(*p);
        free(code);
    }
};


struct LottieFont
{
    enum Origin : uint8_t { Local = 0, CssURL, ScriptURL, FontURL, Embedded };

    ~LottieFont()
    {
        for (auto c = chars.begin(); c < chars.end(); ++c) delete(*c);
        free(style);
        free(family);
        free(name);
    }

    Array<LottieGlyph*> chars;
    char* name = nullptr;
    char* family = nullptr;
    char* style = nullptr;
    float ascent = 0.0f;
    Origin origin = Embedded;
};

struct LottieMarker
{
    char* name = nullptr;
    float time = 0.0f;
    float duration = 0.0f;
    
    ~LottieMarker()
    {
        free(name);
    }
};

struct LottieText : LottieObject
{
    void prepare()
    {
        LottieObject::type = LottieObject::Text;
    }

    void override(LottieProperty* prop) override
    {
        this->doc = *static_cast<LottieTextDoc*>(prop);
        this->prepare();
    }

    LottieTextDoc doc;
    LottieFont* font;
    LottieFloat spacing = 0.0f;  //letter spacing
};


struct LottieTrimpath : LottieObject
{
    enum Type : uint8_t { Simultaneous = 1, Individual = 2 };

    void prepare()
    {
        LottieObject::type = LottieObject::Trimpath;
    }

    bool mergeable() override
    {
        if (!start.frames && start.value == 0.0f && !end.frames && end.value == 100.0f && !offset.frames && offset.value == 0.0f) return true;
        return false;
    }

    void segment(float frameNo, float& start, float& end, LottieExpressions* exps);

    LottieFloat start = 0.0f;
    LottieFloat end = 100.0f;
    LottieFloat offset = 0.0f;
    Type type = Simultaneous;
};


struct LottieShape : LottieObject
{
    virtual ~LottieShape() {}
    uint8_t direction = 0;   //0: clockwise, 2: counter-clockwise, 3: xor(?)

    bool mergeable() override
    {
        return true;
    }
};


struct LottieRoundedCorner : LottieObject
{
    void prepare()
    {
        LottieObject::type = LottieObject::RoundedCorner;
    }
    LottieFloat radius = 0.0f;
};


struct LottiePath : LottieShape
{
    void prepare()
    {
        LottieObject::type = LottieObject::Path;
    }

    LottiePathSet pathset;
};


struct LottieRect : LottieShape
{
    void prepare()
    {
        LottieObject::type = LottieObject::Rect;
    }

    LottiePosition position = Point{0.0f, 0.0f};
    LottiePoint size = Point{0.0f, 0.0f};
    LottieFloat radius = 0.0f;       //rounded corner radius
};


struct LottiePolyStar : LottieShape
{
    enum Type : uint8_t {Star = 1, Polygon};

    void prepare()
    {
        LottieObject::type = LottieObject::Polystar;
    }

    LottiePosition position = Point{0.0f, 0.0f};
    LottieFloat innerRadius = 0.0f;
    LottieFloat outerRadius = 0.0f;
    LottieFloat innerRoundness = 0.0f;
    LottieFloat outerRoundness = 0.0f;
    LottieFloat rotation = 0.0f;
    LottieFloat ptsCnt = 0.0f;
    Type type = Polygon;
};


struct LottieEllipse : LottieShape
{
    void prepare()
    {
        LottieObject::type = LottieObject::Ellipse;
    }

    LottiePosition position = Point{0.0f, 0.0f};
    LottiePoint size = Point{0.0f, 0.0f};
};


struct LottieTransform : LottieObject
{
    struct SeparateCoord
    {
        LottieFloat x = 0.0f;
        LottieFloat y = 0.0f;
    };

    struct RotationEx
    {
        LottieFloat x = 0.0f;
        LottieFloat y = 0.0f;
    };

    ~LottieTransform()
    {
        delete(coords);
        delete(rotationEx);
    }

    void prepare()
    {
        LottieObject::type = LottieObject::Transform;
    }

    bool mergeable() override
    {
        if (!opacity.frames && opacity.value == 255) return true;
        return false;
    }

    LottiePosition position = Point{0.0f, 0.0f};
    LottieFloat rotation = 0.0f;           //z rotation
    LottiePoint scale = Point{100.0f, 100.0f};
    LottiePoint anchor = Point{0.0f, 0.0f};
    LottieOpacity opacity = 255;
    LottieFloat skewAngle = 0.0f;
    LottieFloat skewAxis = 0.0f;

    SeparateCoord* coords = nullptr;       //either a position or separate coordinates
    RotationEx* rotationEx = nullptr;      //extension for 3d rotation
};


struct LottieSolid : LottieObject 
{
    LottieColor color = RGB24{255, 255, 255};
    LottieOpacity opacity = 255;
};


struct LottieSolidStroke : LottieSolid, LottieStroke
{
    void prepare()
    {
        LottieObject::type = LottieObject::SolidStroke;
    }

    void override(LottieProperty* prop) override
    {
        this->color = *static_cast<LottieColor*>(prop);
        this->prepare();
    }
};


struct LottieSolidFill : LottieSolid
{
    void prepare()
    {
        LottieObject::type = LottieObject::SolidFill;
    }

    void override(LottieProperty* prop) override
    {
        this->color = *static_cast<LottieColor*>(prop);
        this->prepare();
    }

    FillRule rule = FillRule::Winding;
};


struct LottieGradient : LottieObject
{
    bool prepare()
    {
        if (!colorStops.populated) {
            if (colorStops.frames) {
                for (auto v = colorStops.frames->begin(); v < colorStops.frames->end(); ++v) {
                    colorStops.count = populate(v->value);
                }
            } else {
                colorStops.count = populate(colorStops.value);
            }
        }
        if (start.frames || end.frames || height.frames || angle.frames || opacity.frames || colorStops.frames) return true;
        return false;
    }

    uint32_t populate(ColorStop& color);
    Fill* fill(float frameNo, LottieExpressions* exps);

    LottiePoint start = Point{0.0f, 0.0f};
    LottiePoint end = Point{0.0f, 0.0f};
    LottieFloat height = 0.0f;
    LottieFloat angle = 0.0f;
    LottieOpacity opacity = 255;
    LottieColorStop colorStops;
    uint8_t id = 0;    //1: linear, 2: radial
};


struct LottieGradientFill : LottieGradient
{
    void prepare()
    {
        LottieObject::type = LottieObject::GradientFill;
        LottieGradient::prepare();
    }

    void override(LottieProperty* prop) override
    {
        this->colorStops = *static_cast<LottieColorStop*>(prop);
        this->prepare();
    }

    FillRule rule = FillRule::Winding;
};


struct LottieGradientStroke : LottieGradient, LottieStroke
{
    void prepare()
    {
        LottieObject::type = LottieObject::GradientStroke;
        LottieGradient::prepare();
    }

    void override(LottieProperty* prop) override
    {
        this->colorStops = *static_cast<LottieColorStop*>(prop);
        this->prepare();
    }
};


struct LottieImage : LottieObject
{
    union {
        char* b64Data = nullptr;
        char* path;
    };
    char* mimeType = nullptr;
    uint32_t size = 0;

    Picture* picture = nullptr;   //tvg render data

    ~LottieImage();

    void prepare()
    {
        LottieObject::type = LottieObject::Image;
    }
};


struct LottieRepeater : LottieObject
{
    void prepare()
    {
        LottieObject::type = LottieObject::Repeater;
    }

    LottieFloat copies = 0.0f;
    LottieFloat offset = 0.0f;

    //Transform
    LottiePosition position = Point{0.0f, 0.0f};
    LottieFloat rotation = 0.0f;
    LottiePoint scale = Point{100.0f, 100.0f};
    LottiePoint anchor = Point{0.0f, 0.0f};
    LottieOpacity startOpacity = 255;
    LottieOpacity endOpacity = 255;
    bool inorder = true;        //true: higher,  false: lower
};


struct LottieGroup : LottieObject
{
    virtual ~LottieGroup()
    {
        for (auto p = children.begin(); p < children.end(); ++p) delete(*p);
    }

    void prepare(LottieObject::Type type = LottieObject::Group);
    bool mergeable() override { return allowMerge; }

    LottieObject* content(const char* id)
    {
        if (name && !strcmp(name, id)) return this;

        //source has children, find recursively.
        for (auto c = children.begin(); c < children.end(); ++c) {
            auto child = *c;
            if (child->type == LottieObject::Type::Group || child->type == LottieObject::Type::Layer) {
                if (auto ret = static_cast<LottieGroup*>(child)->content(id)) return ret;
            } else if (child->name && !strcmp(child->name, id)) return child;
        }
        return nullptr;
    }

    Scene* scene = nullptr;               //tvg render data
    Array<LottieObject*> children;

    bool reqFragment = false;   //requirement to fragment the render context
    bool buildDone = false;     //completed in building the composition.
    bool allowMerge = true;     //if this group is consisted of simple (transformed) shapes.
    bool trimpath = false;      //this group has a trimpath.
};


struct LottieLayer : LottieGroup
{
    enum Type : uint8_t {Precomp = 0, Solid, Image, Null, Shape, Text};

    ~LottieLayer();

    uint8_t opacity(float frameNo)
    {
        //return zero if the visibility is false.
        if (type == Null) return 255;
        return transform->opacity(frameNo);
    }

    bool mergeable() override { return false; }

    void prepare();
    float remap(float frameNo, LottieExpressions* exp);

    struct {
        CompositeMethod type = CompositeMethod::None;
        LottieLayer* target = nullptr;
    } matte;

    BlendMethod blendMethod = BlendMethod::Normal;
    LottieLayer* parent = nullptr;
    LottieFloat timeRemap = 0.0f;
    LottieComposition* comp = nullptr;
    LottieTransform* transform = nullptr;
    Array<LottieMask*> masks;
    RGB24 color;  //used by Solid layer

    float timeStretch = 1.0f;
    float w = 0.0f, h = 0.0f;
    float inFrame = 0.0f;
    float outFrame = 0.0f;
    float startFrame = 0.0f;
    char* refId = nullptr;      //pre-composition reference.
    int16_t pid = -1;           //id of the parent layer.
    int16_t id = -1;            //id of the current layer.

    //cached data
    struct {
        float frameNo = -1.0f;
        Matrix matrix;
        uint8_t opacity;
    } cache;

    Type type = Null;
    bool autoOrient = false;
    bool matteSrc = false;
};


struct LottieSlot
{
    struct Pair {
        LottieObject* obj;
        LottieProperty* prop;
    };

    void assign(LottieObject* target);
    void reset();

    LottieSlot(char* sid, LottieObject* obj, LottieProperty::Type type) : sid(sid), type(type)
    {
        pairs.push({obj, 0});
    }

    ~LottieSlot()
    {
        free(sid);
        if (!overridden) return;
        for (auto pair = pairs.begin(); pair < pairs.end(); ++pair) {
            delete(pair->prop);
        }
    }

    char* sid;
    Array<Pair> pairs;
    LottieProperty::Type type;
    bool overridden = false;
};


struct LottieComposition
{
    ~LottieComposition();

    float duration() const
    {
        return frameCnt() / frameRate;  // in second
    }

    float frameAtTime(float timeInSec) const
    {
        auto p = timeInSec / duration();
        if (p < 0.0f) p = 0.0f;
        return p * frameCnt();
    }

    float timeAtFrame(float frameNo)
    {
        return (frameNo - startFrame) / frameRate;
    }

    float frameCnt() const
    {
        return endFrame - startFrame;
    }

    LottieLayer* layer(const char* name)
    {
        for (auto child = root->children.begin(); child < root->children.end(); ++child) {
            auto layer = static_cast<LottieLayer*>(*child);
            if (layer->name && !strcmp(layer->name, name)) return layer;
        }
        return nullptr;
    }

    LottieLayer* layer(int16_t id)
    {
        for (auto child = root->children.begin(); child < root->children.end(); ++child) {
            auto layer = static_cast<LottieLayer*>(*child);
            if (layer->id == id) return layer;
        }
        return nullptr;
    }

    LottieLayer* asset(const char* name)
    {
        for (auto asset = assets.begin(); asset < assets.end(); ++asset) {
            auto layer = static_cast<LottieLayer*>(*asset);
            if (layer->name && !strcmp(layer->name, name)) return layer;
        }
        return nullptr;
    }

    LottieLayer* root = nullptr;
    char* version = nullptr;
    char* name = nullptr;
    float w, h;
    float startFrame, endFrame;
    float frameRate;
    Array<LottieObject*> assets;
    Array<LottieInterpolator*> interpolators;
    Array<LottieFont*> fonts;
    Array<LottieSlot*> slots;
    Array<LottieMarker*> markers;
    bool expressions = false;
    bool initiated = false;
};

#endif //_TVG_LOTTIE_MODEL_H_

#endif /* LV_USE_THORVG_INTERNAL */

