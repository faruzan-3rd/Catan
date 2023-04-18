#ifndef __CTN__BOARD_HPP
#define __CTN__BOARD_HPP

#include <map>
#include <vector>
#include <complex>
#include <random>
#include "wastils/config.hpp"
#include "wastils/operator.hpp"
#include "wastils/UI.hpp"
#include "wastils/geometry.hpp"
#include "catan/assets.hpp"
#include "catan/constants.hpp"
#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"

typedef sf::Vector2f vec2f; 
typedef std::string str;

namespace ctn{
    class Harbor{
        std::string required_mat;
        int required_num;
        double rotation;
        vec2f 
            position,
            mat_position;
        
    
        public:
        Harbor():
            required_mat{ctn::NONE},
            required_num{-1},
            rotation{0},
            position{vec2f(0, 0)},
            mat_position{vec2f(0, 0)}
        {}
        Harbor(
            const std::string& req_mat_, 
            const int& req_num_, 
            const vec2f& mat_offset_, 
            const vec2f& pl1, 
            const vec2f& pl2,
            const vec2f& position_);

        vec2f get_position() const {return position; }
        vec2f get_mat_position() const {return mat_position; }
        std::string get_required_mat() const {return required_mat; }
        int get_required_num() const {return required_num; }
        double get_rotation() const {return rotation; }
    };
    
    class Place{
        vec2f pos;
        std::vector<int> connected_to;
        std::vector<std::string> available_resources;
        int harbor_id;
        int id;
        str
            house_type,
            color;

        public:
        Place():
            pos{vec2f(0, 0)},
            connected_to{{}},
            available_resources{{}},
            harbor_id{-1},
            id{-1},
            house_type{ctn::NONE},
            color{ctn::GRAY}
        {}
        Place(
                const vec2f& pos_, 
                const std::vector<int>& connected_to_, 
                const int& id_,
                const str& color_):
            pos{pos_},
            connected_to{connected_to_},
            id{id_},
            color{color_}
        {
        }

        bool is_clicked(const vec2f& mouse_pos);
        vec2f get_position() const {return pos; }
        int get_harbor() const {return harbor_id; }
        void set_harbor(int harbor_id_) {harbor_id = harbor_id_; }
        int get_id() const {return id; }
        void add_resource(const std::string& resource) {available_resources.push_back(resource); }
        str get_color() const {return color; }
        str get_type() const {return house_type; }
    };


    class Path{
        str 
            path_type,
            color;
        vec2f pos;


        public:
        Path():
            path_type{ctn::NONE},
            color{ctn::GRAY}
        {}
        Path(
            const str& path_type_, 
            const vec2f& pos_,
            const str& color_){
            path_type = path_type_; pos = pos_; color = color_;
        }

        bool is_clicked(const vec2f& mouse_pos);
        vec2f get_position() const {return pos; }
        str get_path_type() const {return path_type; }
        str get_color() const {return color; }
    };


    struct PathData{
        int to;
        int path_id;

        PathData(){}
        PathData(int to_, int path_id_){
            to = to_; path_id = path_id_;
        }
    };

    class BoardTile{
        std::string tile_type;
        vec2f pos;
        int token;

        public:
        BoardTile():
            tile_type{ctn::NONE},
            pos{vec2f(0, 0)},
            token{0}
        {}
        BoardTile(
                const std::string& tile_type_, 
                const vec2f& pos_):
            tile_type{tile_type_},
            pos{pos_},
            token{0}
        {
        }

        vec2f get_position() const {return pos; }
        int get_token() const {return token; }
        void set_token(const int& token_) {token = token_; }
        std::string get_tile_type() const {return tile_type; }

    };
    
    class Board{
        YAML::Node config;

        std::vector<ctn::Place> places;
        std::vector<ctn::Harbor> harbors;
        std::vector<ctn::Path> paths;
        std::vector<ctn::BoardTile> tiles;
        std::vector<std::vector<ctn::PathData>> graph;

        public:
        Board(){};

        void generate_board(const YAML::Node& config_);
        void generate_graph();
        void generate_harbors();
        void generate_tiles(const YAML::Node& config_);
        void attribute_resources();
        int get_clicked_place(const vec2f& mouse_pos);
        int get_clicked_path(const vec2f& mouse_pos);

        std::vector<ctn::Place> get_places(){return places; }
        std::vector<ctn::Harbor> get_harbors(){return harbors; }
        std::vector<ctn::Path> get_paths(){return paths; }
        std::vector<ctn::BoardTile> get_tiles(){return tiles; }

        private:
        void make_path_if_exist(Place& pl1, Place& pl2, const std::vector<vec2f>& directions);
        vec2f is_connected(Place& pl1, Place& pl2, const std::vector<vec2f>& directions, int max_radius=5);
        std::string get_path_type(const vec2f& dir) const;
    };
}


#endif
