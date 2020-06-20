#include "os_platform.h"
#include "game.h"

global_var v3 camera_pos;
global_var f32 camera_rot = 0.0f;

extern "C" UPDATE_AND_RENDER(updateAndRender)
{

    MemoryStorage memory = game_root.memory_storage;
    
    if (!game_root.game_state) {
        game_root.game_state = alloc<GameState>(memory.game_partition);
    }

    GameState *game_state = game_root.game_state;
    if (game_state->renderer_data.empty()) {
        game_state->renderer_data.reserve(2); //TODO: test code, remove
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
       
        std::shared_ptr<VertexArray> square_vao = VertexArray::instance(game_root.renderer_api);
        square_vao->create(); 
		f32 square_vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

        std::shared_ptr<VertexBuffer> square_vbo = VertexBuffer::instance(game_root.renderer_api);
        square_vbo->create(square_vertices, sizeof(square_vertices));
        std::vector<Element> s_elems;
        s_elems  = {
            {Float3, "a_Position"},
        };
        
        square_vbo->setLayout(s_elems);
        square_vao->addBuffer(square_vbo);


        u32 square_indices[6] = {0,1,2,2,3,0};
        std::shared_ptr<IndexBuffer> square_ibo = IndexBuffer::instance(game_root.renderer_api);
        square_ibo->create(square_indices, ARRAY_LEN(square_indices));
        square_vao->setIndexBuffer(square_ibo);

        const char *vertex = "#version 410 core\n"
            "layout(location = 0) in vec3 a_Position;\n"
            "layout(location = 1) in vec4 a_Color;\n"
            "uniform mat4 u_ViewProjection;\n"
            "uniform mat4 u_Model;\n"
            "out vec4 vColor;\n"
            "void main()\n"
            "{\n"
            "vColor = a_Color;\n"
            "gl_Position =  u_ViewProjection * u_Model *  vec4(a_Position, 1.0);\n"
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
        
        const char *s_vertex = "#version 410 core\n"
            "layout(location = 0) in vec3 a_Position;\n"
            "uniform mat4 u_ViewProjection;\n"
            "uniform mat4 u_Model;\n"
            "out vec3 v_Position;\n"
            "void main()\n"
            "{\n"
            "v_Position = a_Position;\n"
            "gl_Position =  u_ViewProjection * u_Model * vec4(a_Position, 1.0);\n"
            "}\n\0";

        const char *s_fragment = "#version 410 core\n"
            "layout (location = 0) out vec4 color;\n"
            "in vec3 v_Position;\n"
            "void main()\n"
            "{\n"
            "color = vec4(0.2, 0.3, 0.8, 1.0);\n"
            "}\n\0";

        std::shared_ptr<Shader> s_shader = Shader::instance(game_root.renderer_api);
        s_shader->createProgram(s_vertex, s_fragment);
        
        RendererData *renderer_data = alloc<RendererData>(memory.game_partition); 
        renderer_data->indices_count = ARRAY_LEN(indices);
        renderer_data->vao = vao;  
        renderer_data->shader = shader;
        
        RendererData *s_renderer_data = alloc<RendererData>(memory.game_partition); 
        s_renderer_data->indices_count = ARRAY_LEN(square_indices);
        s_renderer_data->vao = square_vao;  
        s_renderer_data->shader = s_shader;

        game_state->renderer_data.push_back(renderer_data);
        game_state->renderer_data.push_back(s_renderer_data);

        RendererCommands commands{game_root.renderer_api}; 
        game_state->renderer = alloc<Renderer>(memory.game_partition);
        game_state->renderer->commands = commands; 
    }   
    
    f32 camera_move_speed = 5.0f;    
    for (int controller_index = 0; controller_index < ARRAY_LEN(input->controllers); ++controller_index) {
        GameControllerInput *controller_input = getController(input, controller_index);
        
        if (controller_input->move_up.ended_down) {
            camera_pos.y -= camera_move_speed * input->dt_for_frame;    
        }

        if (controller_input->move_down.ended_down) {
            camera_pos.y += camera_move_speed * input->dt_for_frame;    
        }

        if (controller_input->move_right.ended_down) {
            camera_pos.x -= camera_move_speed * input->dt_for_frame;    
        }

        if (controller_input->move_left.ended_down) {
            camera_pos.x += camera_move_speed * input->dt_for_frame;    
        }

        if (controller_input->action_up.ended_down) {
            camera_rot += 1.0f;
        }
        
        if (controller_input->action_down.ended_down) {
            camera_rot -= 1.0f;
        }
    } 
   
    Renderer *renderer = game_state->renderer;
    renderer->commands.clear({0.5f, 0.5f, 0.2f});
    
    RendererData *triangle = game_state->renderer_data[0];
    RendererData *square = game_state->renderer_data[1];
   
    game_state->camera.setPosition(camera_pos); 
    game_state->camera.setRotation(camera_rot);

    renderer->beginScene(game_state->camera);
   

    Transform scale_t = scale(0.1f, 0.1f, 0.1f);
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            v3 pos = {x * 1.11f, y * 1.11f, 0.0f};
            Transform t = translate(pos) * scale_t;
            renderer->submit(square->vao, square->shader, t.matrix);
        }
    }

    renderer->submit(triangle->vao, triangle->shader, Mat4x4());
    renderer->endScene();

   return 0; 
}
