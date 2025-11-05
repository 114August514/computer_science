#include <format>
#include "walk.h"

namespace DATA_STRUCTURE {

void walk::print_para() {
    std::cout << steps << std::endl;
    for (int i = 0; i < gold_num; i++) {
        std::cout << "Gold" << i  
                  << ": (" << gold[i].x << "," << gold[i].y << ")"
                  << "; Value: " << gold_value[i] << std::endl;
    }
}

int walk::compute_distance(int i, const Point& pos) {
    // TODO
    int target_x = gold[i].x; 
    int target_y = gold[i].y;

    int diff_x = abs(target_x - pos.x);
    int diff_y = abs(target_y - pos.y);

    int steps_x = (diff_x + 1) / 2;
    int steps_y = (diff_y + 1) / 2;
    return steps_x + steps_y;
}

int walk::get_value() {
    if (gold_num <= 0) return 0;

    int max_value = 0;
    // TODO
    // 初始化
    directory<VisitSet, int> visited{};
    q.clear();

    q.push(element{});
    visited.set(VisitSet{}, 0);

    // 核心算法
    while (!q.empty())
    {
        element current = q.top();
        q.pop();

        int* visited_steps = visited.find({current.pos, current.get_gold});
        if (visited_steps && *visited_steps < current.steps) continue;

        if (current.value > max_value) max_value = current.value;
        
        for (int i = 0; i < gold_num; i++)
        {
            if (current.get_gold.find(i)) continue;
            
            int next_steps = current.steps + compute_distance(i, current.pos);
            if (next_steps > steps) continue;

            element next{};
            next.steps = next_steps;
            next.value = current.value + gold_value[i];
            next.pos = gold[i];
            next.get_gold = current.get_gold;
            next.get_gold.insert(i);

            VisitSet next_visited = {next.pos, next.get_gold};

            int* visited_steps = visited.find(next_visited);
            if (visited_steps == nullptr || next_steps < *visited_steps)
            {
                visited.set(next_visited, next_steps);
                q.push(next);
            }
        }
    }
    return max_value;
}

}