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
    struct Port{
        sf::Sprite sprite, resource_sprite;
        sf::RenderWindow* window;

        std::string required_mat;
        int required_num;
    
        Port();
        Port(sf::Sprite sprite_, sf::Sprite resource_sprite_, sf::RenderWindow* window_, std::string req_mat_, int req_num_, vec2f mat_offset_, vec2f pl1, vec2f pl2);

        void draw();
    };
    
    class Place{
        sf::RenderWindow* window;
        vec2f pos;
        std::vector<int> connected_to;
        std::vector<ctn::Tile> available_resources;
        int port_id;

        sf::Sprite sprite;
        int id;

        public:
        Place();
        Place(sf::RenderWindow* window_, sf::Sprite sprite, vec2f coords, std::vector<int> connected_to_, int id_);

        void draw();
        int get_id() const;
        vec2f get_position() const;
        void add_resource(ctn::Tile resource);
        void set_port(int port_id_);
        void set_sprite(sf::Sprite sprite_);
        bool is_clicked(const vec2f& mouse_pos);
    };


    struct Path{
        sf::Sprite sprite;
        std::string path_typ;

        Path(){}
        Path(sf::Sprite sp, std::string path_typ_){
            sprite = sp; path_typ = path_typ_;
        }

        void set_sprite(sf::Sprite sprite_);
        bool is_clicked(const vec2f& mouse_pos);
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
        std::map<std::string, sf::Sprite> sprites, port_mat_sprites;
        vec2f u, v;
        sf::Texture texture, env_texture;
        YAML::Node config, assets_cfg;

        sf::Font font;
        was::Text* house_id = nullptr;

        std::vector<ctn::Place> places;
        std::vector<ctn::Port> ports;
        std::vector<std::vector<ctn::PathData>> graph;
        std::vector<ctn::Path> path_rend;

        public:
        Board();
        ~Board();

        void load_assets(YAML::Node config_, sf::RenderWindow* window_);
        void generate_board();
        void generate_graph();
        void generate_ports();
        void draw();
        void attribute_resources(const std::vector<ctn::BoardTile>& tiles);
        void click(const vec2f& mouse_pos);

        private:
        void make_path_if_exist(Place& pl1, Place& pl2, const std::vector<vec2f>& directions);
        vec2f is_connected(Place& pl1, Place& pl2, const std::vector<vec2f>& directions, int max_radius=5);
        std::string get_path_type(vec2f dir);
    };
}


#endif
