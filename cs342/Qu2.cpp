#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>


using namespace std;
int col=0;
int suc=0;
int bac=0;
struct Node {
    int id;
    int backoff;
    int collisions;
    int successfulTransmissions;
    int backoffCount; 

    Node(int node_id) {
        id = node_id;
        backoff = 0;
        collisions = 0;
        successfulTransmissions = 0;
        backoffCount = 0; 
    }

    void attemptTransmission(int &channelStatus) {
        if (channelStatus == 0) {
            suc++;
            cout << "Node " << id << " transmitting." << endl;
            channelStatus=1;
            successfulTransmissions++;
        } else {
            backoff=rand() % 10;
            cout << "Node " << id << " backing off for " << backoff << " time units." << endl;
            backoffCount++; 
            bac++;
        }
    }

    void handleCollision() {
        cout << "Collision detected for Node " << id << ". Retrying after random backoff." << endl;
        collisions++;
        backoff = rand() % 10; 
        backoffCount++;
        bac++;
    }
};

int main() {
    srand(time(0));

    int N;
    cout << "Enter the number of Nodes: ";
    cin >> N;

    int simulationTime;
    cout << "Enter the simulation time: ";
    cin >> simulationTime; 

    int channelStatus = 0; 

    vector<Node> nodes;
    for (int i = 0; i < N; i++) {
        nodes.push_back(Node(i));
    }

    for (int timeUnit = 0; timeUnit < simulationTime; timeUnit++) {
        cout << "Time Unit " << timeUnit << endl;

        channelStatus = 0;
        
        int transmittingNodes = 0;
        if(timeUnit==0){
            for ( Node &node : nodes) {
            if (node.backoff == 0 ) {
                transmittingNodes++;
                node.attemptTransmission(channelStatus);
            }
             if (transmittingNodes > 1) {
            col++;
            for (Node &node : nodes) {
                if (node.backoff == 0) {
                    node.handleCollision();
                }
            }
        } else {
            for (Node &node : nodes) {
                if (node.backoff == 0) {
                    if(channelStatus==0)
                     {}
                    else
                    cout << "Channel is busy Node " << node.id << " backing off "<< endl;

                    node.attemptTransmission(channelStatus);


                } else {
                    node.backoff--;
                }
            }

        }
        }
        }else{
        for (const Node &node : nodes) {
            if (node.backoff == 0 ) {
                
                transmittingNodes++;
            }

        }

        if (transmittingNodes > 1) {
            col++;
            for (Node &node : nodes) {
                if (node.backoff == 0) {
                    node.handleCollision();
                }
            }
        } else {
            for (Node &node : nodes) {
                if (node.backoff == 0) {
                    if(channelStatus==0)
                     {}
                    else
                    cout << "Channel is busy Node " << node.id << " backing off "<< endl;

                    node.attemptTransmission(channelStatus);


                } else {
                    node.backoff--;
                }
            }
        }}
        
    }
    
    cout << "\nNode Statistics:" << endl;
       for (const Node& node : nodes) {
        cout << "Node " << node.id << " - Successful Transmissions: " << node.successfulTransmissions
             << ", Collisions: " << node.collisions << ", Backoff Count: " << node.backoffCount << endl;
    }
        cout<< " Total - Successful Transmissions: " << suc << ", Collisions: " << col << ", Backoff Count: " << bac << endl;
    

    return 0;
}
