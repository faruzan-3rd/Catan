#ifndef __CTN__BOARD_HPP
#define __CTN__BOARD_HPP

#include <map>
#include <vector>
#include <complex>
#include "wastils/config.hpp"
#include "wastils/operator.hpp"
#include "wastils/UI.hpp"
#include "wastils/geometry.hpp"
#include "catan/assets.hpp"
#include "catan/constants.hpp"
#include "catan/tiles.hpp"
#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"

typedef sf::Vector2f vec2f;

namespace ctn{
    class Harbor{
        sf::Sprite sprite, resource_sprite;
        sf::RenderWindow* window;

        std::string required_mat;
        int required_num;
    
        public:
        Harbor():
            sprite{sf::Sprite()},
            resource_sprite{sf::Sprite()},
            window{nullptr},
            required_mat{ctn::NONE},
            required_num{-1}
        {}
        Harbor(
            sf::RenderWindow* window_, 
            const sf::Sprite& sprite_,
            const sf::Sprite& resource_sprite_,
            const std::string& req_mat_, 
            const int& req_num_, 
            const vec2f& mat_offset_, 
            const vec2f& pl1, 
            const vec2f& pl2);

        void draw(){window->draw(sprite); window->draw(resource_sprite); }
        vec2f getPosition() const {return sprite.getPosition(); }
        std::string get_required_mat() const {return required_mat; }
        int get_required_num() const {return required_num; }
    };
    
    class Place{
        sf::RenderWindow* window;
        vec2f pos;
        std::vector<int> connected_to;
        std::vector<std::string> available_resources;
        int harbor_id;

        sf::Sprite sprite;
        int id;

        public:
        Place():
            window{nullptr},
            pos{vec2f(0, 0)},
            connected_to{{}},
            available_resources{{}},
            harbor_id{-1},
            sprite{sf::Sprite()},
            id{-1}
        {}
        Place(
                sf::RenderWindow* window_, 
                const sf::Sprite& sprite_, 
                const vec2f& pos_, 
                const std::vector<int>& connected_to_, 
                const int& id_):
            window{window_},
            sprite{sprite_},
            pos{pos_},
            connected_to{connected_to_},
            id{id_}
        {
            sprite.setPosition(pos);
        }

        void draw(){window->draw(sprite); }
        bool is_clicked(const vec2f& mouse_pos);
        vec2f getPosition() const {return sprite.getPosition(); }
        int get_harbor() const {return harbor_id; }
        void set_harbor(int harbor_id_) {harbor_id = harbor_id_; }
        int get_id() const {return id; }
        void set_sprite(const sf::Sprite& sprite_) {sprite = sprite_; sprite.setPosition(pos); }
        void add_resource(const std::string& resource) {available_resources.push_back(resource); }
    };


    class Path{
        sf::RenderWindow* window;
        sf::Sprite sprite;
        std::string path_type;
        vec2f pos;


        public:
        Path():
            window{nullptr},
            sprite{sf::Sprite()},
            path_type{ctn::NONE}
        {}
        Path(sf::RenderWindow* window_, const sf::Sprite& sp, const std::string& path_type_, const vec2f& pos_){
            window = window_; sprite = sp; path_type = path_type_; pos = pos_; sprite.setPosition(pos);
        }

        void draw(){window->draw(sprite); }
        bool is_clicked(const vec2f& mouse_pos);
        vec2f getPosition() const {return sprite.getPosition(); }
        std::string get_path_type() const {return path_type; }
        void set_sprite(const sf::Sprite& sprite_) {sprite = sprite_; sprite.setPosition(pos); }
    };


    struct PathData{
        int to;
        int path_id;

        PathData(){}
        PathData(int to_, int path_id_){
            to = to_; path_id = path_id_;
        }
    };
    
    class Board{
        sf::RenderWindow* window;
        std::map<std::string, sf::Sprite> sprites, harbor_mat_sprites;
        vec2f u, v;
        sf::Texture texture, env_texture;
        YAML::Node config, assets_cfg;

        sf::Font font;
        was::Text* house_id = nullptr;

        std::vector<ctn::Place> places;
        std::vector<ctn::Harbor> harbors;
        std::vector<std::vector<ctn::PathData>> graph;
        std::vector<ctn::Path> path_rend;

        public:
        Board(){};
        ~Board(){if(house_id != nullptr) delete house_id; }

        void load_assets(const YAML::Node& config_, sf::RenderWindow* window_);
        void generate_board();
        void generate_graph();
        void generate_harbors();
        void draw();
        void attribute_resources(const std::vector<ctn::BoardTile>& tiles);
        int get_clicked_place(const vec2f& mouse_pos);
        int get_clicked_path(const vec2f& mouse_pos);
        void make_building_at(const int& id, const std::string& b_type);
        void make_path_at(const int& id, const std::string& color);

        private:
        void make_path_if_exist(Place& pl1, Place& pl2, const std::vector<vec2f>& directions);
        vec2f is_connected(Place& pl1, Place& pl2, const std::vector<vec2f>& directions, int max_radius=5);
        std::string get_path_type(const vec2f& dir) const;
    };
}


#endif
