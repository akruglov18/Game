#include <gtest/gtest.h>
#include "creature.h"
#include "enemies/enemy.h"
#include "player/player.h"
#include "skills_graph.h"

namespace {

void foop(Player& p) {
    p.get_experience();
}

void fooe(std::vector<Enemy>& e) {
    e.reserve(2);
}

void foof(Field& f) {
    f.get_height();
}

TEST(Skill, can_create) {
    EXPECT_NO_THROW(Skill(foop, fooe, foof));
}

TEST(Node, can_create) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    EXPECT_NO_THROW(Node("name", skill, 0, sf::FloatRect()));
}

TEST(Node, locking_0) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    Node node("name", skill, 0, sf::FloatRect());
    EXPECT_FALSE(node.is_locked());
}

TEST(Node, locking_1) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node->add_child(node_child);
    EXPECT_FALSE(node->is_locked());
}

TEST(Node, locking_2) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node->add_child(node_child);
    EXPECT_TRUE(node_child->is_locked());
}

TEST(Node, locking_3) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node->add_child(node_child);
    node->unlock();
    EXPECT_FALSE(node_child->is_locked());
}

TEST(Node, locking_4) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node->add_child(node_child);
    EXPECT_ANY_THROW(node_child->unlock());
}

TEST(Node, multiple_locking_0) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child_1(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child_2(new Node("name", skill, 0, sf::FloatRect()));
    node->add_childs({node_child_1, node_child_2});
    EXPECT_TRUE(node_child_1->is_locked() && node_child_2->is_locked());
}

TEST(Node, multiple_locking_1) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child_1(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child_2(new Node("name", skill, 0, sf::FloatRect()));
    node->add_childs({node_child_1, node_child_2});
    node->unlock();
    EXPECT_FALSE(node_child_1->is_locked() || node_child_2->is_locked());
}

TEST(Node, multiple_locking_2) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node_1(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_2(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node_1->add_child(node_child);
    node_2->add_child(node_child);
    EXPECT_TRUE(node_child->is_locked());
}

TEST(Node, multiple_locking_3) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node_1(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_2(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node_1->add_child(node_child);
    node_2->add_child(node_child);
    node_1->unlock();
    EXPECT_TRUE(node_child->is_locked());
}

TEST(Node, multiple_locking_4) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node_1(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_2(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node_1->add_child(node_child);
    node_2->add_child(node_child);
    node_2->unlock();
    EXPECT_TRUE(node_child->is_locked());
}

TEST(Node, multiple_locking_5) {
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::shared_ptr<Node> node_1(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_2(new Node("name", skill, 0, sf::FloatRect()));
    std::shared_ptr<Node> node_child(new Node("name", skill, 0, sf::FloatRect()));
    node_1->add_child(node_child);
    node_2->add_child(node_child);
    node_1->unlock();
    node_2->unlock();
    EXPECT_FALSE(node_child->is_locked());
}

TEST(SkillsGraph, can_create) {
    ASSERT_NO_THROW(SkillsGraph());
}

TEST(SkillsGraph, right_id) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    ASSERT_EQ(0, graph.add_skill("name", skill, sf::FloatRect(), 0));
}

TEST(SkillsGraph, locking_0) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    ASSERT_FALSE(graph.is_locked(root));
}

TEST(SkillsGraph, locking_1) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    ASSERT_TRUE(graph.is_locked(child));
}

TEST(SkillsGraph, locking_2) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    graph.unlock(0);
    ASSERT_FALSE(graph.is_locked(child));
}

TEST(SkillsGraph, locking_3) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    ASSERT_ANY_THROW(graph.unlock(child));
}

TEST(SkillsGraph, multuple_locking_0) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    ASSERT_FALSE(graph.is_locked(root));
}

TEST(SkillsGraph, multuple_locking_1) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child1 = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    std::size_t child2 = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    ASSERT_TRUE(graph.is_locked(child1) && graph.is_locked(child2));
}

TEST(SkillsGraph, multuple_locking_2) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child1 = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    std::size_t child2 = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root));
    graph.unlock(root);
    ASSERT_FALSE(graph.is_locked(child1) || graph.is_locked(child2));
}

TEST(SkillsGraph, multuple_locking_3) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root1 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t root2 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root1));
    graph.connect(root2, child);
    ASSERT_TRUE(graph.is_locked(child));
}

TEST(SkillsGraph, multuple_locking_4) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root1 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t root2 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root1));
    graph.connect(root2, child);
    graph.unlock(root1);
    ASSERT_TRUE(graph.is_locked(child));
}

TEST(SkillsGraph, multuple_locking_5) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root1 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t root2 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root1));
    graph.connect(root2, child);
    graph.unlock(root2);
    ASSERT_TRUE(graph.is_locked(child));
}

TEST(SkillsGraph, multuple_locking_6) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root1 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t root2 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root1));
    graph.connect(root2, child);
    graph.unlock(root1);
    graph.unlock(root2);
    ASSERT_FALSE(graph.is_locked(child));
}

TEST(SkillsGraph, multuple_locking_7) {
    SkillsGraph graph;
    std::shared_ptr<Skill> skill(new Skill(foop));
    std::size_t root1 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t root2 = graph.add_skill("name", skill, sf::FloatRect(), 0);
    std::size_t child = graph.add_skill("name", skill, sf::FloatRect(), 0, static_cast<int>(root1));
    graph.connect(root2, child);
    ASSERT_FALSE(graph.is_locked(root1) || graph.is_locked(root2));
}

}  // namespace
