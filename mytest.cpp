/*
 * File:        mytest.cpp
 * Author:      Joey Hardester
 * Date:        10/6/21
 * Section:     04
 * E-mail:      josephh5@umbc.edu
 * Description: This file is to help test the functions of the Swarm class.
 */

#include "swarm.h"
#include <random>
#include <iomanip>

enum RANDOM {UNIFORM, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORM) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 and standard deviation of 20
            m_normdist = std::normal_distribution<>(50,20);
        }
        else{
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
    }

    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else{
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//uniform distribution

};

class Tester{
public:
    void testDuplicates() {
        cout << "Testing insertion of duplicates" << endl;
        Swarm test;
        Robot testRobot(10000, DRONE);
        cout << "Inserting first robot with ID: 10000" << endl;
        test.insert(testRobot);
        test.listRobots();
        Robot testRobot2(10000,DRONE);
        cout << "Inserting second robot with ID: 10000" << endl;
        test.insert(testRobot2);
        test.listRobots();
    }

    bool testBST(Swarm* s) {
        return testBST(s->m_root);
    }

    bool testBST(Robot* aBot) {
        if(aBot->m_right != nullptr) {
            if(aBot->m_id > aBot->m_right->m_id) {
                return false;
            }
        }
        if(aBot->m_left != nullptr) {
            if(aBot->m_id < aBot->m_left->m_id) {
                return false;
            }
        }
        if(aBot->m_right != nullptr) {
            if(!(testBST(aBot->m_right)))
                return false;
        }
        if(aBot->m_left != nullptr) {
            if(!(testBST(aBot->m_left)))
                return false;
        }
        return true;
    }

    void testRemoveDead() {
        Swarm test;
        cout << "Testing removeDead(): " << endl;
        Robot robot1(15000, DRONE);
        test.insert(robot1);
        Robot robot2(30000, DRONE);
        test.insert(robot2);
        Robot robot3(10000, DRONE);
        test.insert(robot3);
        Robot robot4(40000, DRONE);
        test.insert(robot4);
        cout << "Tree before removeDead: " << endl;
        test.dumpTree();
        cout << endl;

        cout << "Setting Robot with ID: 30000 to DEAD State" << endl;
        test.setState(30000, DEAD);
        test.removeDead();

        cout << "Tree after removeDead: "  << endl;
        test.dumpTree();
        cout << endl;
    }

    void testInsert() {
        Random idGen(MINID,MAXID);
        Random typeGen(0,4);
        Swarm bigTree;
        clock_t start, stop;
        cout << "Measuring time to insert 5000 items into a tree: " << endl;
        int bigTreeSize = 5000;
        int ID = 0;
        double T = 0.0;
        start = clock();
        for(int i=0;i<bigTreeSize;i++) {
            ID = idGen.getRandNum();
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            bigTree.insert(robot);
        }
        stop = clock();
        T = stop - start;
        cout << "Inserting 5000 items took: " << T << " clock ticks!" << endl;

        cout << "Measuring time to insert 1000 items into a tree: " << endl;
        Swarm litTree;
        int litTreeSize = 1000;
        int litID = 0;
        double litT = 0.0;
        start = clock();
        for(int i=0;i<litTreeSize;i++) {
            litID = idGen.getRandNum();
            Robot robot(litID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            litTree.insert(robot);
        }
        stop = clock();
        litT = stop - start;
        cout << "Inserting 1000 items took: " << litT << " clock ticks!" << endl;
    }

    void testRemove() {
        Random idGen(MINID,MAXID);
        Random typeGen(0,4);
        Swarm bigTree;
        clock_t start, stop;
        cout << "Measuring time to remove an item in a tree with 5000 items: " << endl;
        int bigTreeSize = 5000;
        int ID = 0;
        double T = 0.0;
        for(int i=0;i<bigTreeSize;i++) {
            ID = idGen.getRandNum();
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            bigTree.insert(robot);
        }
        cout << "Removing root bot: " << endl;
        start = clock();
        bigTree.remove(bigTree.m_root->m_id);
        stop = clock();
        T = stop - start;
        cout << "Removal of root took " << T << " clock ticks!" << endl;

        cout << "Measuring time to remove an item in a tree with 1000 items: " << endl;
        Swarm litTree;
        int litTreeSize = 1000;
        ID = 0;
        T = 0.0;
        for(int i=0;i<litTreeSize;i++) {
            ID = idGen.getRandNum();
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            litTree.insert(robot);
        }
        cout << "Removing root bot:" << endl;
        start = clock();
        litTree.remove(litTree.m_root->m_id);
        stop = clock();
        T = stop - start;
        cout << "Removal of root took " << T << " clock ticks!" << endl;
    }

};

int main(){
    Tester tester;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    {
        Swarm team;
        int teamSize = 10;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }


        cout << "\nDump after inserting " << teamSize << " nodes:\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after inserting " << teamSize << " nodes:\n";
        team.listRobots();
        cout << endl;

        cout << "Testing properties of BST after insertion:" << endl;
        if(tester.testBST(&team))
            cout << "Tree is valid" << endl;
        else
            cout << "Tree is not valid" << endl;


        team.remove(tempID);
        cout << "\nDump after removing the node with ID: " << tempID << "\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after removing the node with ID: " << tempID << "\n";
        team.listRobots();
        cout << endl;

        cout << "Testing properties of BST after removal:" << endl;
        if(tester.testBST(&team))
            cout << "Tree is valid" << endl;
        else
            cout << "Tree is not valid" << endl;

        cout << endl;
        tester.testDuplicates();

        cout << endl;
        tester.testRemoveDead();

        cout << endl;
        cout << "Measuring time complexity for insertion: " << endl;
        tester.testInsert();

        cout << endl;
        cout << "Measuring time complexity for removal: " << endl;
        tester.testRemove();
    }
    return 0;
}