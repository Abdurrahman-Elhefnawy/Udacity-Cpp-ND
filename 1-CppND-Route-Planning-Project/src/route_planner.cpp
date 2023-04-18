#include "route_planner.h"
#include "route_model.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    
    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node =  &m_Model.FindClosestNode(start_x, start_y); 
    end_node =  &m_Model.FindClosestNode(end_x, end_y); 
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    
    return node->distance(*end_node);

}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    
    for(auto& neighbor : current_node->neighbors){
        neighbor->h_value = CalculateHValue(neighbor);
        neighbor->g_value = current_node->g_value + neighbor->distance(*current_node);
        neighbor->parent = current_node;

        open_list.push_back(neighbor);
        neighbor->visited = true;
    }
}

RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end() ,[](const auto& first, const auto& second ){
        return (first->h_value + first->g_value) < (second->h_value + second->g_value);
    });

    auto first_node = open_list.front();
    open_list.erase(open_list.begin());
    return first_node;


}




std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while (current_node->parent != nullptr ) {
        path_found.push_back(*current_node);
        distance = current_node->distance(*(current_node->parent));
        current_node = current_node->parent;
    }
    path_found.push_back(*current_node);

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


void RoutePlanner::AStarSearch() {

    RouteModel::Node* current_node =nullptr;
    // TODO: Implement your solution here.
    start_node->visited = true;
    open_list.push_back(start_node);

    while (open_list.size() > 0) {
        current_node = NextNode();
        if(current_node->distance(*end_node) == 0){
            m_Model.path = ConstructFinalPath(end_node);
            return;
        }
        else
            AddNeighbors(current_node);
    }
    
    
    return;
}