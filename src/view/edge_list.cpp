#include <sgl/view/edge_list.h>
#include <algorithm>



sgl::view::edge_list::edge_list(bool oriented, bool weighted)
    : view(oriented, weighted)
{

}



sgl::view::edge_list::~edge_list()
{

}



void sgl::view::edge_list::add_node()
{
    sgl::node_set_t::const_iterator it = this->nodes.end();

    if(this->nodes.size() != 0)
    {
        it = --this->nodes.end();
    }

    sgl::node_id_t node_id = 0;

    if(it != this->nodes.end())
    {
        node_id = it->get_id() + 1;
    }

    this->nodes.insert(node_id);
}



void sgl::view::edge_list::add_edge(const sgl::edge& edge)
{
    this->nodes.insert(edge.get_first());
    this->nodes.insert(edge.get_second());
    this->edges.insert(edge);
}



void sgl::view::edge_list::remove_node(const sgl::node& node)
{
   sgl::edge_set_t::const_iterator it_begin =
        std::find_if(this->edges.begin(), this->edges.end(),
            [&node](const sgl::edge& edge)
            {
                if(edge.get_first() == node)
                {
                    return true;
                }

                return false;
            });

    sgl::edge_set_t::const_iterator it_end =
        std::find_if(this->edges.begin(), this->edges.end(),
            [&node](const sgl::edge& edge)
            {
                if(edge.get_second() == node)
                {
                    return true;
                }

                return false;
            });

    if(it_begin != this->edges.end())
    {
        this->edges.erase(it_begin);
    }

    if(it_end != this->edges.end())
    {
        this->edges.erase(it_end);
    }

    this->nodes.erase(node);
}



void sgl::view::edge_list::remove_edge(const sgl::edge& edge)
{
    this->edges.erase(edge);
}



sgl::view::type sgl::view::edge_list::get_type() const
{
    return sgl::view::type::edge_list;
}



sgl::node_set_t sgl::view::edge_list::get_nodes() const
{
    return this->nodes;
}



sgl::edge_set_t sgl::view::edge_list::get_edges() const
{
    return this->edges;
}



bool sgl::view::edge_list::exists(const sgl::edge& edge) const
{
    sgl::edge_set_t::const_iterator it_forward = this->edges.find(edge);

    sgl::edge_set_t::const_iterator it_backward = this->edges.end();
    if(!this->is_oriented())
    {
        sgl::edge edge_backward(edge.get_second(), edge.get_first(), edge.get_weight());
        it_backward = this->edges.find(edge_backward);
    }
    
    if(it_forward != this->edges.end() || it_backward != this->edges.end())
    {
        return true;
    }

    return false;
}



bool sgl::view::edge_list::exists(const sgl::node& node) const
{
    return this->nodes.find(node) != this->nodes.end();
}
