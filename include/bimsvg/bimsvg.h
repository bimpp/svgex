#pragma once

#include <string>
#include <vector>
#include <array>

#include <svgpp/svgpp.hpp>

template<typename T>
struct bimsvg
{
    union node
    {
        struct
        {
            T x;
            T y;
        };
        std::array<T, 2> d;

        node(T _x = 0, T _y = 0)
            : x(_x)
            , y(_y)
        {
            //
        }
    };

    struct wall
    {
        size_t  start_node_id;
        size_t  end_node_id;
        T       thickness;

        wall(size_t _start_node_id = 0, size_t _end_node_id = 0, size_t _thickness = 0)
            : start_node_id(_start_node_id)
            , end_node_id(_end_node_id)
            , thickness(_thickness)
        {
            //
        }

        bool is_valid() const
        {
            return (start_node_id != end_node_id);
        }
    };

    struct hole
    {
        std::string name;
        std::string type;
        std::string direction;
        size_t      wall_id;
        T           distance;
        T           width;

        hole()
            : name("unknown")
            , wall_id(0)
            , distance(0)
            , width(0)
        {
            //
        }
    };

    struct area
    {
        std::string name;
        std::vector<size_t> wall_ids;

        area()
            : name("unknown")
            , wall_ids()
        {
            //
        }
    };

    struct plan
    {
        std::string name;
        std::vector<node> nodes;
        std::vector<wall> walls;
        std::vector<area> areas;

        plan()
            : name("unknown")
            , nodes()
            , walls()
            , areas()
        {
            //
        }

        bool is_valid() const
        {
            //
            return !areas.empty();
        }

        void reset()
        {
            name = "unknown";
            nodes.clear();
            walls.clear();
            areas.clear();
        }

        bool area_wall_ids(size_t _area_id, std::vector<size_t>& _wall_ids) const
        {
            //
            return false;
        }

        bool area_node_ids(size_t _area_id, std::vector<size_t>& _node_ids) const
        {
            //
            return false;
        }
    };

    static bool load_from_string(const std::string& _svg, plan& _plan)
    {
        //
        return false;
    }
};
