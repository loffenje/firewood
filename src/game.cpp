#include "os_platform.h"
#include "game.h"

extern "C" UPDATE_AND_RENDER(updateAndRender)
{

    MemoryStorage memory = game_root.memory_storage;
    
    if (!game_root.game_state) {
        game_root.game_state = alloc<GameState>(memory.game_partition);
    }

    GameState *game_state = game_root.game_state;
    if (!game_state->renderer_data) {
#if 0
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

#endif   
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
        
        const char *vertex = "#version 410 core\n"
            "layout(location = 0) in vec3 a_Position;\n"
            "layout(location = 1) in vec4 a_Color;\n"
            "uniform mat4 u_ViewProjection;\n"
            "out vec4 vColor;\n"
            "void main()\n"
            "{\n"
            "vColor = a_Color;\n"
            "gl_Position =  u_ViewProjection * vec4(a_Position, 1.0);\n"
            "}\n\0";

        const char *fragment = "#version 410 core\n"
            "layout (location = 0) out vec4 color;\n"
            "in vec4 vColor;\n"
            "void main()\n"
            "{\n"
            "color = vColor;\n"
            "}\n\0";
        
        std::shared_ptr<Shader> shader = Shader::instance(game_root.renderer_api);
        shader->createProgram(vertex, fragment);
        
        game_state->renderer_data = alloc<RendererData>(memory.game_partition); 
        game_state->renderer_data->indices_count = ARRAY_LEN(indices);
        game_state->renderer_data->vao = vao;  
        game_state->renderer_data->shader = shader;
    }   
    
    RendererCommands commands{game_root.renderer_api}; 
    commands.clear({0.5f, 0.1f, 0.2f});

    beginScene();
    game_state->renderer_data->shader->uploadUniformMat4("u_ViewProjection", game_state->camera.view_projection_mat);
    game_state->renderer_data->shader->bind();
    submit(commands, game_state->renderer_data->vao);
    endScene();

   return 0; 
}
