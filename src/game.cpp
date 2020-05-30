#include "os_platform.h"
#include "game.h"

global_var bool initialized = false;
global_var RendererData renderer_data = {};

extern "C" UPDATE_AND_RENDER(updateAndRender)
{
    if (!initialized) {
        TestEntity entity1;
        entity1.init();

        TestEntity entity2;
        entity2.init();
        
        std::shared_ptr<EntityDestroyed> destroyed_event(new EntityDestroyed());

        EventDispatcher::instance()->queueEvent(destroyed_event);
        EventDispatcher::instance()->abortEvent(EntityDestroyed::event_type);
        EventDispatcher::instance()->queueEvent(destroyed_event);
        EventDispatcher::instance()->broadcast(60000);


        entity1.destroy();
        entity2.destroy();

        
        std::shared_ptr<VertexArray> vao = VertexArray::instance(game_root.renderer_api);
        vao->create();

        f32 vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
    
        
        std::shared_ptr<VertexBuffer> vbo = VertexBuffer::instance(game_root.renderer_api);
        vbo->create(vertices, sizeof(vertices));
        std::vector<Element> elems;
        elems  = {
            {Float3, "a_Position"},
            {Float4, "a_Color"}
        };
        
        vbo->setLayout(elems);
        vao->addBuffer(vbo);

        u32 indices[3] = {0,1,2};
      
        std::shared_ptr<IndexBuffer> ibo = IndexBuffer::instance(game_root.renderer_api);
        ibo->create(indices, ARRAY_LEN(indices));
        vao->setIndexBuffer(ibo);
        
        const char *vertex = "#version 330 core\n"
            "layout(location = 0) in vec3 a_Position;\n"
            "layout(location = 1) in vec4 a_Color;\n"
            "out vec4 vColor;\n"
            "void main()\n"
            "{\n"
            "vColor = a_Color;\n"
            "gl_Position =  vec4(a_Position, 1.0);\n"
            "}\n\0";

        const char *fragment = "#version 330 core\n"
            "layout (location = 0) out vec4 color;\n"
            "in vec4 vColor;\n"
            "void main()\n"
            "{\n"
            "color = vColor;\n"
            "}\n\0";
        
        std::shared_ptr<Shader> shader = Shader::instance(game_root.renderer_api);
        shader->createProgram(vertex, fragment);
      
        renderer_data.indices_count = ARRAY_LEN(indices);
        renderer_data.vao = vao;  
        renderer_data.shader = shader;
               
        initialized = true;
    }   
    
    game_root.renderer_api->clear({0.5f, 0.1f, 0.2f});
    renderer_data.shader->bind();
    renderer_data.vao->bind();
    glDrawElements(GL_TRIANGLES, renderer_data.indices_count, GL_UNSIGNED_INT, nullptr);

   return 0; 
}
