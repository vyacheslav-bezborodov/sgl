#include <algorithm>
#include <sgl/view/adjacency_matrix.h>



sgl::view::adjacency_matrix::adjacency_matrix(std::size_t nodes, bool oriented, bool weighted)
    : base_t(oriented, weighted)
    , matrix(nodes, matrix_row_t(nodes))
{

}



sgl::view::adjacency_matrix::adjacency_matrix(const sgl::view::view& other)
    : base_t(other.is_oriented(), other.is_weighted())
{
    *this = other;
}



sgl::view::adjacency_matrix::~adjacency_matrix()
{

}



void sgl::view::adjacency_matrix::add_node(const sgl::core::node& node)
{
    matrix_t::size_type diff = this->matrix.size() - node.get_id() + 1;

    if(diff > 0)
    {
        std::for_each(this->matrix.begin(), this->matrix.end(),
        [&diff](matrix_row_t& row)
        {
            row.insert(row.end(), diff, 0);
        });

        this->matrix.push_back(matrix_row_t(this->matrix.size() + diff, 0));
    }
}



void sgl::view::adjacency_matrix::add_edge(const sgl::core::edge& edge)
{
    sgl::core::node_id_t from = edge.get_first().get_id();
    sgl::core::node_id_t to = edge.get_second().get_id();
    sgl::core::weight_t weight = edge.get_weight();

    if(!this->in_range(from, to))
    {
        throw std::out_of_range("adjacency_matrix::add_edge: "
            "can't add edge: the node with node_id = " +
            std::to_string(std::max(from, to)) +
            " doesn't exists");
    }

    if(weight > 0)
    {
        this->matrix[from][to] = weight;

        if(!this->is_oriented())
        {
            this->matrix[to][from] = weight;
        }
    }
}



void sgl::view::adjacency_matrix::remove_node(const sgl::core::node& node)
{
    if(!this->exists(node))
    {
        throw std::invalid_argument("adjacency_matrix::remove_node: "
            "can't remove node: the node with node_id = " +
            std::to_string(node.get_id()) +
            " doesn't exists");
    }

    matrix_t::iterator it = this->matrix.begin() + node.get_id();
    this->matrix.erase(it);

    std::for_each(this->matrix.begin(), this->matrix.end(),
        [&](matrix_row_t& row)
        {
            row.erase(row.begin() + node.get_id());
        });
}



void sgl::view::adjacency_matrix::remove_edge(const sgl::core::edge& edge)
{
    sgl::core::node_id_t from = edge.get_first().get_id();
    sgl::core::node_id_t to = edge.get_second().get_id();

    if(!this->in_range(from, to))
    {
        throw std::out_of_range("adjacency_matrix::remove_edge: "
            "can't remove edge (" + edge.to_string() + "): "
            "the node with node_id = " +
            std::to_string(std::max(from, to)) +
            " doesn't exists");
    }

    this->matrix[from][to] = 0;

    if(!this->is_oriented())
    {
        this->matrix[to][from] = 0;
    }
}



sgl::view::type sgl::view::adjacency_matrix::get_type() const
{
    return sgl::view::type::adjacency_matrix;
}



sgl::core::node_set_t sgl::view::adjacency_matrix::get_nodes() const
{
    sgl::core::node_set_t nodes;

    for(sgl::core::node_id_t node_id = 0; node_id < this->matrix.size(); ++node_id)
    {
        nodes.insert(node_id);
    }

    return nodes;
}



sgl::core::edge_set_t sgl::view::adjacency_matrix::get_edges() const
{
    sgl::core::edge_set_t edges;

    for(sgl::core::node_id_t first = 0; first < this->matrix.size(); ++first)
    {
        for(sgl::core::node_id_t second = 0; second < this->matrix[first].size(); ++second)
        {
            sgl::core::weight_t weight = this->matrix[first][second];

            if(weight != 0)
            {
                sgl::core::edge edge_forward(first, second, weight);
                sgl::core::edge edge_backward(second, first, weight);

                sgl::core::edge_set_t::const_iterator it_forward = edges.find(edge_forward);
                sgl::core::edge_set_t::const_iterator it_backward = edges.find(edge_backward);

                if(it_forward == edges.end() && it_backward == edges.end())
                {
                    edges.insert(edge_forward);
                }
            }
        }
    }

    return edges;
}



bool sgl::view::adjacency_matrix::exists(const sgl::core::edge& edge) const
{
    bool exists = true;

    sgl::core::node_id_t from = edge.get_first().get_id();
    sgl::core::node_id_t to = edge.get_second().get_id();

    if(this->in_range(from, to))
    {
        if(this->is_oriented())
        {
            if(this->matrix[from][to] == 0)
            {
                exists = false;
            }
        }
        else
        {
            if(this->matrix[to][from] == 0 || this->matrix[to][from] == 0)
            {
                exists = false;
            }
        }
    }
    else
    {
        exists = false;
    }

    return exists;
}



bool sgl::view::adjacency_matrix::exists(const sgl::core::node& node) const
{
    bool exists = false;

    std::size_t distance = std::distance(
        this->matrix.begin(),
        this->matrix.begin() + node.get_id());

    if(distance < this->matrix.size())
    {
        exists = true;
    }

    return exists;
}



sgl::core::weight_t sgl::view::adjacency_matrix::get_edge_weight(
    const sgl::core::node& first, const sgl::core::node& second) const
{
    sgl::core::node_id_t from = first.get_id();
    sgl::core::node_id_t to = second.get_id();

    if(!this->in_range(from, to))
    {
        throw std::out_of_range("adjacency_matrix::get_weight(): "
            "can't retrieve a weight of edge: "
            "the node with node_id = " +
            std::to_string(std::max(from, to)) +
            " doesn't exists");
    }

    return this->matrix[from][to];
}



bool sgl::view::adjacency_matrix::in_range(
    sgl::core::node_id_t from, sgl::core::node_id_t to) const
{
    sgl::core::node_id_t max = std::max(from, to);

    return max < this->matrix.size();
}



sgl::view::adjacency_matrix& sgl::view::adjacency_matrix::operator=(const sgl::view::adjacency_matrix& rhs)
{
    if(this == &rhs)
    {
        return *this;
    }

    if(this->is_oriented() != rhs.is_oriented() || this->is_weighted() != rhs.is_weighted())
    {
        throw std::invalid_argument("adjacency_matrix::operator=: can't assign: oriented/weighted flags mismatch");
    }

    this->matrix = rhs.matrix;

    return *this;
}



sgl::view::adjacency_matrix& sgl::view::adjacency_matrix::operator=(const sgl::view::view& rhs)
{
    if(this->is_oriented() != rhs.is_oriented() || this->is_weighted() != rhs.is_weighted())
    {
        throw std::invalid_argument("adjacency_matrix::operator=: can't assign: oriented/weighted flags mismatch");
    }

    this->matrix.clear();

    sgl::core::node_set_t nodes = rhs.get_nodes();
    this->matrix.resize(nodes.size(), matrix_row_t(nodes.size()));

    sgl::core::edge_set_t edges = rhs.get_edges();
    for(const sgl::core::edge& edge : edges)
    {
        sgl::core::node_id_t first = edge.get_first().get_id();
        sgl::core::node_id_t second = edge.get_second().get_id();
        sgl::core::weight_t weight = edge.get_weight();

        if(this->is_weighted())
        {
            this->matrix[first][second] = weight;

            if(!this->is_oriented())
            {
                this->matrix[second][first] = weight;
            }
        }
        else
        {
            this->matrix[first][second] = 1;

            if(!this->is_oriented())
            {
                this->matrix[second][first] = 1;
            }
        }
    }

    return *this;
}
