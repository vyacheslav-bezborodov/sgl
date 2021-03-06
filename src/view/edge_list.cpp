#include <sgl/view/edge_list.h>
#include <algorithm>



sgl::view::edge_list::edge_list(bool oriented, bool weighted)
    : base_t(oriented, weighted)
{

}



sgl::view::edge_list::edge_list(const sgl::view::view& other)
    : base_t(other.is_oriented(), other.is_weighted())
{
    *this = other;
}



sgl::view::edge_list::~edge_list()
{

}



void sgl::view::edge_list::add_node(const sgl::core::node& node)
{
    this->nodes.insert(node);
}



void sgl::view::edge_list::add_edge(const sgl::core::edge& edge)
{
    sgl::core::edge_set_t::const_iterator it = std::find_if(this->edges.begin(), this->edges.end(),
        [this, &edge](const sgl::core::edge& item)
        {
            if(item.get_first() == edge.get_first() && item.get_second() == edge.get_second())
            {
                return true;
            }

            if(!this->is_oriented())
            {
                if(item.get_second() == edge.get_first() && item.get_first() == edge.get_second())
                {
                    return true;
                }
            }

            return false;
        });

    if(it != this->edges.end())
    {
        this->edges.erase(it);
    }
    else
    {
        this->nodes.insert(edge.get_first());
        this->nodes.insert(edge.get_second());
    }

    this->edges.insert(edge);
}



void sgl::view::edge_list::remove_node(const sgl::core::node& node)
{
   sgl::core::edge_set_t::const_iterator it_begin =
        std::find_if(this->edges.begin(), this->edges.end(),
            [&node](const sgl::core::edge& edge)
            {
                if(edge.get_first() == node)
                {
                    return true;
                }

                return false;
            });

    sgl::core::edge_set_t::const_iterator it_end =
        std::find_if(this->edges.begin(), this->edges.end(),
            [&node](const sgl::core::edge& edge)
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



void sgl::view::edge_list::remove_edge(const sgl::core::edge& edge)
{
    this->edges.erase(edge);

    if(!this->is_oriented())
    {
        this->edges.erase(sgl::core::edge(edge.get_second(), edge.get_first(), edge.get_weight()));
    }
}



sgl::view::type sgl::view::edge_list::get_type() const
{
    return sgl::view::type::edge_list;
}



sgl::core::node_set_t sgl::view::edge_list::get_nodes() const
{
    return this->nodes;
}



sgl::core::edge_set_t sgl::view::edge_list::get_edges() const
{
    return this->edges;
}



bool sgl::view::edge_list::exists(const sgl::core::edge& edge) const
{
    sgl::core::edge_set_t::const_iterator it_forward = this->edges.find(edge);

    sgl::core::edge_set_t::const_iterator it_backward = this->edges.end();
    if(!this->is_oriented())
    {
        sgl::core::edge edge_backward(edge.get_second(), edge.get_first(), edge.get_weight());
        it_backward = this->edges.find(edge_backward);
    }

    if(it_forward != this->edges.end() || it_backward != this->edges.end())
    {
        return true;
    }

    return false;
}



bool sgl::view::edge_list::exists(const sgl::core::node& node) const
{
    return this->nodes.find(node) != this->nodes.end();
}



sgl::view::edge_list& sgl::view::edge_list::operator=(const sgl::view::edge_list& rhs)
{
    if(this == &rhs)
    {
        return *this;
    }

    if(this->is_oriented() != rhs.is_oriented() || this->is_weighted() != rhs.is_weighted())
    {
        throw std::invalid_argument("edge_list::operator=: can't assign: oriented/weighted flags mismatch");
    }

    this->nodes = rhs.nodes;
    this->edges = rhs.edges;

    return *this;
}



sgl::view::edge_list& sgl::view::edge_list::operator=(const sgl::view::view& rhs)
{
    if(this->is_oriented() != rhs.is_oriented() || this->is_weighted() != rhs.is_weighted())
    {
        throw std::invalid_argument("edge_list::operator=: can't assign: oriented/weighted flags mismatch");
    }

    this->nodes = rhs.get_nodes();
    this->edges = rhs.get_edges();

    return *this;
}
