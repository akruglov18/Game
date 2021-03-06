#pragma once
#include <memory>
#include <vector>
#include "node.h"

class SkillsGraph {
public:
    SkillsGraph();
    void init();
    std::size_t add_skill(const std::string& name, std::shared_ptr<Skill> skill, const sf::FloatRect& _coord, int cost,
                          int parent = -1);
    std::vector<std::size_t> add_skills(const std::vector<std::shared_ptr<Skill>>& skills,
                                        const std::vector<int>& costs, int parent = -1);
    std::vector<std::size_t> add_skills(const std::vector<std::shared_ptr<Skill>>& skills,
                                        const std::vector<int>& costs, const std::vector<int>& parents);

    void connect(std::size_t id_from, std::size_t id_to);
    bool is_locked(std::size_t id);
    std::shared_ptr<Skill> unlock(std::size_t id);

    std::shared_ptr<Node> operator[](const std::size_t index) {
        return skills[index];
    }
    std::size_t size() const {
        return skills.size();
    }
    std::vector<std::shared_ptr<Node>> get() const {
        return skills;
    }

private:
    std::vector<std::shared_ptr<Node>> skills;
};
