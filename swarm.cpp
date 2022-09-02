//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
#include "swarm.h"
/*
 * Constructor for Swarm Object
 */
Swarm::Swarm(){
    m_root = nullptr;
}

/*
 * Destructor for Swarm Object
 */
Swarm::~Swarm(){
    clear();
}

/*
 * Function: insert(const Robot& robot)
 * Description: This function makes a call to the recursive helper function which will allow for quicker insertion.
 */
void Swarm::insert(const Robot& robot){
    m_root = insert(robot.m_id, robot.m_type, robot.m_state, m_root);
}

/*
 * Function: clear()
 * Description: This function makes a call to the recursive helper function which will allow for quicker removal.
 */
void Swarm::clear(){
    clear(m_root);
}

/*
 * Function: remove(int id)
 * Description: This function makes a call to the recursive helper function which will allow for quicker removal.
 */
void Swarm::remove(int id) {
    remove(id, m_root);
}

/*
 * Function: updateHeight(Robot* aBot)
 * Description: This function updates the height of the bot that is passed in by looking at the heights of its children.
 */
void Swarm::updateHeight(Robot* aBot){
    if(aBot == nullptr) {
        return;
    }

    int leftHeight = -1; //default, if left child does not exist
    if(aBot->m_left != nullptr) {
        leftHeight = aBot->m_left->m_height;
    }

    int rightHeight = -1; //default, if right child does not exist
    if(aBot->m_right != nullptr) {
        rightHeight = aBot->m_right->m_height;
    }

    //updates height appropriately
    if(leftHeight >= rightHeight) {
        aBot->m_height = 1 + leftHeight;
    } else {
        aBot->m_height = 1 + rightHeight;
    }
}

/*
 * Function: checkImbalance(Robot* aBot)
 * Description: This function checks if the bot passed in has a height imbalance which would cause the need for a
 *              rotation to occur.
 */
int Swarm::checkImbalance(Robot* aBot){
    if(aBot == nullptr) {
        return -1;
    } else {
        int leftChildHeight = -1; //default heights, if either child does not exist
        int rightChildHeight = -1;
        if(aBot->m_left != nullptr)
            leftChildHeight = aBot->m_left->m_height;
        if(aBot->m_right != nullptr)
            rightChildHeight = aBot->m_right->m_height;
        return (leftChildHeight - rightChildHeight); //returns the difference
    }
}

/*
 * Function: rebalance(Robot* aBot)
 * Description: This function will rebalance the bot that is passed in where a rebalance needs to occur.
 */
Robot* Swarm::rebalance(Robot* aBot){
    if((checkImbalance(aBot) < -1) && (checkImbalance(aBot->m_right) <= 0))
        //if right child has greater height and right child of right child has greater height
        return leftRotation(aBot);
    else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->m_left) >= 0))
        //if left child has greater height and left child of left child has greater height
        return rightRotation(aBot);
    else if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->m_right) >= 0)) {
        //if right child has greater height and left child of right child has greater height
        aBot->m_right = rightRotation(aBot->m_right);
        return leftRotation(aBot);
    }
    else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->m_left) <= 0)) {
        //if left child has greater height and right child of left child has greater height
        aBot->m_left = leftRotation(aBot->m_left);
        return rightRotation(aBot);
    }
    else
        //no rotation needs to occur
        return aBot;
}

/*
 * Function: listRobots()
 * Description: This function makes a call to the recursive helper function to print the robots and their information.
 */
void Swarm::listRobots() const {
    listRobots(m_root);
}

/*
 * Function: setState(int id, STATE state)
 * Description: This function finds the intended bot where the state change needs to occur then does the change.
 */
bool Swarm::setState(int id, STATE state){
    if(findBot(id)) {
        Robot* temp = getBot(id, m_root);
        temp->m_state = state;
        return true;

    }
    return false;
}

/*
 * Function: removeDead()
 * Description: This function makes a call to the recursive helper function to remove any dead bots from the tree.
 */
void Swarm::removeDead() {
    removeDead(m_root);
}

/*
 * Function: findBot(int id)
 * Description: This function makes a call to the recursive helper function to see if the intended bot exist.
 */
bool Swarm::findBot(int id) const {
    if(findBot(id, m_root))
        return true;
    return false;
}

/*
 * Function: dumpTree() const
 * Description: This function makes a call to the recursive helper function to print the following nodes.
 */
void Swarm::dumpTree() const {
    dump(m_root);
}

/*
 * Function: dump(Robot* aBot) const
 * Description: This function prints the data out in a in order fashion.
 */
void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

/*
 * Function: listRobots(Robot* aBot) const
 * Description: This function prints the data out in a in order fashion.
 */
void Swarm::listRobots(Robot* aBot) const{
    if (aBot != nullptr){
        listRobots(aBot->m_left);
        cout << aBot->m_id << ":";
        cout << aBot->getStateStr() << ":";
        cout << aBot->getTypeStr() << ":" << endl;
        listRobots(aBot->m_right);
    }
}

/*
 * Function: getBot(int id, Robot* aBot)
 * Description: This function searches the tree in order to find the intended bot.
 */
Robot* Swarm::getBot(int id, Robot* aBot) {
    if (aBot == nullptr || aBot->m_id == id) {
        return aBot;
    } else if (aBot->m_id > id) {
        return getBot(id, aBot->m_left);
    } else {
        return getBot(id, aBot->m_right);
    }
}

/*
 * Function: findBot(int id, Robot* aBot) const
 * Description: This function searches the tree to see if a specific value exist.
 */
bool Swarm::findBot(int id, Robot* aBot) const{
    if(aBot == nullptr) {
        return false;
    } else if(aBot->m_id == id) {
        return true;
    } else if(aBot->m_id > id) {
        return findBot(id, aBot->m_left);
    } else {
        return findBot(id, aBot->m_right);
    }
}

/*
 * Function: removeDead(Robot* aBot)
 * Description: This function traverses the tree in order to remove any dead bots.
 */
void Swarm::removeDead(Robot* aBot) {
    if(aBot->getStateStr() == "DEAD") {
        remove(aBot->m_id);
    } else {
        if(aBot->m_left != nullptr) {
            removeDead(aBot->m_left);
        }
        if(aBot->m_right != nullptr) {
            removeDead(aBot->m_right);
        }
    }
}

/*
 * Function: insert(int id, ROBOTTYPE type, STATE state, Robot*& aBot)
 * Description: This function is to insert the nodes by the value of their id
 * Note: This is in O(log n) due to the fact that not every node is visited when inserting.
 *       For example, if the value is greater than the root, none of the left subtree will be visited during the
 *       insertion process
 */
Robot* Swarm::insert(int id, ROBOTTYPE type, STATE state, Robot*& aBot) {
    if(aBot == nullptr) {
        Robot* newRobot = new Robot(id, type, state);
        aBot = newRobot;
        aBot->m_left = nullptr;
        aBot->m_right = nullptr;
        return newRobot;
    } else if(id < aBot->m_id) {
        aBot->m_left = insert(id, type, state, aBot->m_left);
        updateHeight(aBot);
        return rebalance(aBot);
    } else if (id > aBot->m_id) {
        aBot->m_right = insert(id, type, state, aBot->m_right);
        updateHeight(aBot);
        return rebalance(aBot);
    } else {
        return aBot;
    }
}

/*
 * Function: clear(Robot* aBot)
 * Description: This function is to clear the tree of all nodes and delete all memory properly.
 */
void Swarm::clear(Robot* aBot) {
    if(aBot == nullptr) {
        return;
    } else {
        clear(aBot->m_left);
        clear(aBot->m_right);
        delete aBot;
    }
}

/*
 * Function: getMin(Robot* aBot)
 * Description: This function gets the leftmost value of the node that is passed in.
 */
Robot* Swarm::getMin(Robot* aBot) {
    if(aBot->m_left == nullptr) {
        return aBot;
    } else {
        return getMin(aBot->m_left);
    }
}

/*
 * Function: remove(int id, Robot*& aBot)
 * Description: This function is to remove the intended bot from the tree
 * Note: This is in O(log n) due to the fact that not every node is visited when inserting.
 *       For example, if the value is less than the root, none of the right subtree will be visited during the
 *       removal process
 */
Robot* Swarm::remove(int id, Robot*& aBot) {
    Robot* temp;
    if(aBot == nullptr) {
        return nullptr;
    } else if (id < aBot->m_id) {
        aBot->m_left = remove(id, aBot->m_left);
    } else if (id > aBot->m_id) {
        aBot->m_right = remove(id, aBot->m_right);
    } else if (aBot->m_left != nullptr && aBot->m_right != nullptr) {
        //case of two children
        temp = getMin(aBot->m_right);
        aBot->m_id = temp->m_id;
        aBot->m_type = temp->m_type;
        aBot->m_state = temp->m_state;
        //bot that was rotated needs to be removed, removal is done (leftmost value of right node)
        aBot->m_right = remove(aBot->m_id, aBot->m_right);
    } else {
        temp = aBot;
        //case of one child or no children
        if(aBot->m_left == nullptr) {
            aBot = aBot->m_right;
        } else if(aBot->m_right == nullptr) {
            aBot = aBot->m_left;
        }
        delete temp;
    }
    updateHeight(aBot);
    aBot = rebalance(aBot);
    return aBot;
}

/*
 * Function: leftRotation(Robot* aBot)
 * Description: This function performs a left rotation on the tree when balancing. Once done, the function
 *              returns the right node of the original bot that was passed in(since that was where the rotation
 *              occurred).
 */
Robot* Swarm::leftRotation(Robot* aBot) {
    Robot* a = aBot;
    Robot* b = a->m_right;
    a->m_right = b->m_left;
    b->m_left = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

/*
 * Function: rightRotation(Robot* aBot)
 * Description: This function performs a right rotation on the tree when balancing. Once done, the function
 *              returns the left node of the original bot that was passed in(since that was where the rotation
 *              occurred).
 */
Robot* Swarm::rightRotation(Robot* aBot) {
    Robot* a = aBot;
    Robot* b = a->m_left;
    a->m_left = b->m_right;
    b->m_right = a;
    updateHeight(a);
    updateHeight(b);
    return b;

}