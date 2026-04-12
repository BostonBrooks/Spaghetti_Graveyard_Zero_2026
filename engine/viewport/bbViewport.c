#include "engine/viewport/bbViewport.h"
#include "engine/graphics/bbColours.h"

extern bbMapCoords testGoalPoint;

bbFlag bbRenderable_init(bbRenderable* renderable, I32 height, I32 width){

    renderable->renderTexture
    = sfRenderTexture_create(width, height, sfFalse);
    sfRenderTexture_clear(renderable->renderTexture, sfTransparent);
    bbAssert(renderable->renderTexture != NULL, "bad sfml\n");
    renderable->texture = sfRenderTexture_getTexture(renderable->renderTexture);

    bbAssert(renderable->texture != NULL, "bad sfml\n");
    renderable->sprite = sfSprite_create();

    bbAssert(renderable->sprite != NULL, "bad sfml\n");
    sfSprite_setTexture(renderable->sprite, renderable->texture, sfTrue);

    return Success;
}

bbFlag bbViewport_init(bbViewport* viewport, I32 height, I32 width)
{
    bbViewport* VP = viewport;

    bbRenderable_init(&VP->background, height, width);
    bbRenderable_init(&VP->ground, height, width);
    bbRenderable_init(&VP->main, height, width);
    bbRenderable_init(&VP->mapicon, height, width);

    VP->height = height;
    VP->width = width;


    sfRenderTexture_clear(VP->background.renderTexture, sfBlue);

    VP->viewpoint.i = 10000;
    VP->viewpoint.j = 10000;
    VP->viewpoint.k = 0;

    testGoalPoint.i = 10000;
    testGoalPoint.j = 10000;
    testGoalPoint.k = 0;


    char vertShader[] = "\
        uniform vec2 offset;\
        void main()\
        {\
         \
            vec4 offsetvertex = vec4(offset.x, offset.y, 0, 0);\
            gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
         \
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
         \
           gl_FrontColor = gl_Color;\
        }\
    ";

    char fragShader[] = "\
        uniform sampler2D Background;\
        uniform sampler2D Ground;\
        uniform sampler2D Main;\
        uniform sampler2D MapIcon;\
        \
        void main()\
        {\
            vec4 background = texture2D(Background, gl_TexCoord[0].xy);\
            vec4 ground     = texture2D(Ground,     gl_TexCoord[0].xy);\
            vec4 main       = texture2D(Main,       gl_TexCoord[0].xy);\
            vec4 mapicon    = texture2D(MapIcon,    gl_TexCoord[0].xy);\
        \
            vec4 mix1 = mix(background, ground,  ground.a);\
            vec4 mix2 = mix(mix1,       main,    main.a  );\
            vec4 mix3 = mix(mix2,       mapicon, mapicon.a / 2.0  );\
\
            gl_FragColor =  mix3;\
            \
        }\
    ";

    VP->shader = sfShader_createFromMemory(vertShader, NULL, fragShader);

    VP->renderStates.shader = VP->shader;
    VP->renderStates.blendMode = sfBlendAlpha;
    VP->renderStates.transform = sfTransform_Identity;
    VP->renderStates.texture = VP->background.texture;

    sfShader_setTextureUniform(VP->shader,"Background", VP->background.texture);
    sfShader_setTextureUniform(VP->shader,"Ground", VP->ground.texture);
    sfShader_setTextureUniform(VP->shader,"Main", VP->main.texture);
    sfShader_setTextureUniform(VP->shader,"MapIcon", VP->mapicon.texture);

    return Success;
}

bbFlag bbViewport_new(bbViewport** viewport, I32 height, I32 width){
    bbViewport* VP = malloc(sizeof *VP);
    bbAssert(VP != NULL, "bad malloc\n");

    bbRenderable_init(&VP->background, height, width);
    bbRenderable_init(&VP->ground, height, width);
    bbRenderable_init(&VP->main, height, width);
    bbRenderable_init(&VP->mapicon, height, width);

    VP->height = height;
    VP->width = width;


    sfRenderTexture_clear(VP->background.renderTexture, sfBlue);

    VP->viewpoint.i = 10000;
    VP->viewpoint.j = 10000;
    VP->viewpoint.k = 0;

    char vertShader[] = "\
        uniform vec2 offset;\
        void main()\
        {\
         \
            vec4 offsetvertex = vec4(offset.x, offset.y, 0, 0);\
            gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
         \
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
         \
           gl_FrontColor = gl_Color;\
        }\
    ";

    char fragShader[] = "\
        uniform sampler2D Background;\
        uniform sampler2D Ground;\
        uniform sampler2D Main;\
        uniform sampler2D MapIcon;\
        \
        void main()\
        {\
            vec4 background = texture2D(Background, gl_TexCoord[0].xy);\
            vec4 ground     = texture2D(Ground,     gl_TexCoord[0].xy);\
            vec4 main       = texture2D(Main,       gl_TexCoord[0].xy);\
            vec4 mapicon    = texture2D(MapIcon,    gl_TexCoord[0].xy);\
        \
            vec4 mix1 = mix(background, ground,  ground.a);\
            vec4 mix2 = mix(mix1,       main,    main.a  );\
            vec4 mix3 = mix(mix2,       mapicon, mapicon.a / 2.0  );\
\
            gl_FragColor =  mix3;\
            \
        }\
    ";

    VP->shader = sfShader_createFromMemory(vertShader, NULL, fragShader);

    VP->renderStates.shader = VP->shader;
    VP->renderStates.blendMode = sfBlendAlpha;
    VP->renderStates.transform = sfTransform_Identity;
    VP->renderStates.texture = VP->background.texture;

    sfShader_setTextureUniform(VP->shader,"Background", VP->background.texture);
    sfShader_setTextureUniform(VP->shader,"Ground", VP->ground.texture);
    sfShader_setTextureUniform(VP->shader,"Main", VP->main.texture);
    sfShader_setTextureUniform(VP->shader,"MapIcon", VP->mapicon.texture);

    *viewport = VP;

    return Success;
}


bbFlag bbViewport_draw(sfRenderWindow* window, bbViewport* viewport){
    sfRenderTexture_display(viewport->background.renderTexture);
    sfRenderTexture_display(viewport->ground.renderTexture);
    sfRenderTexture_display(viewport->main.renderTexture);
    sfRenderTexture_display(viewport->mapicon.renderTexture);

    sfRenderWindow_drawSprite(window, viewport->background.sprite,
                              &viewport->renderStates);

    return Success;
}


bbFlag bbViewport_clear(bbViewport* viewport){

    sfRenderTexture_clear(viewport->background.renderTexture, bbGreen);
    sfRenderTexture_clear(viewport->ground.renderTexture, sfTransparent);
    sfRenderTexture_clear(viewport->main.renderTexture, sfTransparent);
    sfRenderTexture_clear(viewport->mapicon.renderTexture, sfTransparent);
}