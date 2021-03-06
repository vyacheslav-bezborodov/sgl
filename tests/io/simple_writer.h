#pragma once

#include <sstream>

#include <cxxtest/TestSuite.h>

#include <sgl/io/simple_writer.h>
#include <sgl/view/adjacency_matrix.h>
#include <sgl/view/adjacency_list.h>
#include <sgl/view/edge_list.h>



class simple_writer_test: public CxxTest::TestSuite
{
    std::stringstream stringstream;
    std::string string;
    sgl::io::writer_t writer;
    sgl::view::view_t view;
 
    
    
public:
    virtual void setUp() override
    {
        this->stringstream.str(std::string());
        this->writer = std::make_shared<sgl::io::simple_writer>(this->stringstream);
    }
    
    
    
public:
    void test_write_adjacency_matrix()
    {
        this->init_adjacency_matrix();
        
        TS_ASSERT_THROWS_NOTHING(this->writer->write(this->view));
        TS_ASSERT_EQUALS(this->stringstream.str(), this->string);
    }
    
    
    
    void test_write_adjacency_list()
    {
        this->init_adjacency_list();
        
        TS_ASSERT_THROWS_NOTHING(this->writer->write(this->view));
        TS_ASSERT_EQUALS(this->stringstream.str(), this->string);    
    }
    
    
    
    void test_write_edge_list()
    {
        this->init_edge_list();
        
        TS_ASSERT_THROWS_NOTHING(this->writer->write(this->view));
        TS_ASSERT_EQUALS(this->stringstream.str(), this->string);
    }
    
    
    
private:
    void init_adjacency_matrix()
    {
        this->string =
            "C 3\n"
            "0 0\n"
            "0 2 0\n"
            "2 0 5\n"
            "0 5 0\n";
                
        this->view = std::make_shared<sgl::view::adjacency_matrix>(3);
        
        sgl::core::edge edge0(0, 1, 2);
        sgl::core::edge edge1(1, 2, 5);
        
        this->view->add_edge(edge0);       
        this->view->add_edge(edge1);
    }
    
    
    
    void init_adjacency_list()
    {
        this->string =
            "L 4\n"
            "0 1\n"
            "1 2 3 5\n"
            "0 2\n"
            "\n"
            "0 5\n";
        
        this->view = std::make_shared<sgl::view::adjacency_list>(0, 1);
        
        const std::size_t nodes = 4;
        for(sgl::core::node_id_t node = 0; node < nodes; ++node)
        {
            this->view->add_node(node);
        }
        
        sgl::core::edge edge0(0, 1, 2);
        sgl::core::edge edge1(0, 3, 5);
        
        this->view->add_edge(edge0);       
        this->view->add_edge(edge1);
    }
    
    
    
    void init_edge_list()
    {
        this->string =
            "E 4 2\n"
            "0 1\n"
            "0 1 2\n"
            "0 3 5\n";
        
        this->view = std::make_shared<sgl::view::edge_list>(0, 1);
        
        const std::size_t nodes = 4;
        for(sgl::core::node_id_t node = 0; node < nodes; ++node)
        {
            this->view->add_node(node);
        }
        
        this->view->add_edge(sgl::core::edge(0, 1, 2));
        this->view->add_edge(sgl::core::edge(0, 3, 5));
    }
};
