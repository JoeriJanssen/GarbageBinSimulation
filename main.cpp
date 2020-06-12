// Joeri Janssen, s4630610
// Rob Föllings,  s4144945

#include <bits/stdc++.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#define NIL 0
#define INF INT_MAX

using namespace std;

typedef map<string, int> maptype;

//  Breadth-first search algorithm used in the Hopcroft-Karp algorithm.
bool bfs (int actors, int pairU[], int pairV[], int dist[], int **adj_matrix)
{
    queue<int> Q;

    for (int i = 1; i <= actors; i++)
    {
        if (pairU[i] == NIL)
        {
            dist[i] = 0;
            Q.push(i);
        }
        else
            dist[i] = INF;
    }

    dist[NIL] = INF;

    while(!Q.empty())
    {
        int u = Q.front();
        Q.pop();

        if (dist[u] < dist[NIL])
        {
            for (int i = actors; i < 2 * actors + 1; ++i)
            {
                if (adj_matrix[u][i] > 0)
                {
                    int v = i - actors;
                    if (dist[pairV[v]] == INF)
                    {
                        dist[pairV[v]] = dist[u] + 1;
                        Q.push(pairV[v]);
                    }
                }
            }
        }
    }
    return (dist[NIL] != INF);
}

//  Depth-first search algorithm used in the Hopcroft-Karp algorithm.
bool dfs(int u, int actors, int pairV[], int pairU[], int dist[], int **adj_matrix)
{
    if (u != NIL)
    {
        vector<int> neighbors;

        for (int i = actors; i < 2 * actors + 1; ++i)
        {
            if (adj_matrix[u][i] > 0)
            {
                int v = i - actors;
                neighbors.push_back(v);
            }
        }

//      Sort the neighbors in descending order.
        sort(neighbors.begin(), neighbors.end(), greater<int>());

        for (int i = 0; i < neighbors.size(); i++)
        {
            if (dist[pairV[neighbors[i]]] == dist[u] + 1)
            {
                if (dfs(pairV[neighbors[i]], actors, pairV, pairU, dist, adj_matrix))
                {
                    pairV[neighbors[i]] = u;
                    pairU[u] = neighbors[i];
                    return true;
                }
            }
        }

        dist[u] = INF;
        return false;
    }
    return true;
}

//  Hopcroft-Karp algorithm to calculate the matching of the graph.
int HopcroftKarp(int actors, int **adj_matrix, int pairU[])
{
    int pairV[actors + 1];
    int dist[actors + 1];

    for (int i = 1; i <= actors; i++)
    {
        pairU[i] = NIL;
        pairV[i] = NIL;
    }

    int matching = 0;

    while (bfs(actors, pairU, pairV, dist, adj_matrix))
    {
        for (int i = 1; i <= actors; i++)
            if (pairU[i] == NIL && dfs(i, actors, pairV, pairU, dist, adj_matrix))
                matching++;
    }

    return matching;
}

//  The strategy used if you are playing as Mark.
void strategyMark (int actors, int **adj_matrix, maptype actor_index, string actor_string[])
{
    int pairU[actors + 1];

//  Run the algorithm to calculate the matching.
    int matching = HopcroftKarp(actors, adj_matrix, pairU);

//  If a perfect matching exist you know you can follow the perfect matching you found stored in the array pairU to ensure you will win the game.
    if (matching == actors)
    {
        while(true)
        {
            auto& input = cin;
            string actress;
            input >> actress;

            auto found_actress = actor_index.find(actress);
            int index = pairU[found_actress->second];

            string actor = actor_string[index + actors];

            cout << actor << endl;
        }
    }
    else
    {
        vector<int> adj_actors;
        int winner_index = -1;

        while(true)
        {
            auto& input = cin;
            string actress;
            input >> actress;

            auto found_actress = actor_index.find(actress);
            int index = found_actress->second;

//          Find all neighbors of the given actress in a vector.
            for (int i = actors + 1; i <= 2*actors; i++)
            {
                if(adj_matrix[index][i] > 0)
                {
//                  Check if one of the neighbors results in an immediate loss.
//                  If yes, don't add it to the vector of neighbors.
                    bool valid = true;
                    int neighbors = 0;
                    for (int l = 1; l <= actors; l++)
                    {
                        if (l != index)
                        {
                            if (adj_matrix[l][i] > 0)
                            {
                                for (int k = actors + 1; k <= 2*actors; k++)
                                {
                                    if (adj_matrix[l][k] > 0)
                                        neighbors++;
                                }

                                if (neighbors == 1)
                                    valid = false;

                                neighbors = 0;
                            }
                        }
                    }

                    if (valid)
                        adj_actors.push_back(i);
                }
            }

//          If no more neighbors exist you can't name another actor and you lose the game.
            if (adj_actors.empty())
            {
                cout << "IGiveUp" << endl;
            }
            else
            {
//              Set the connections for the given actress to 0 in the adjacency matrix.
                for (int i = actors + 1; i <= 2*actors; i++)
                    adj_matrix[index][i] = 0;

//              Check if one of the neighbors results in an immediate win and store its index.
                int k = 0;
                int l = 1;
                int neighbors = 0;
                while (winner_index == -1 && k < adj_actors.size())
                {
                    while (neighbors == 0 && l <= actors)
                    {
                        if (adj_matrix[l][adj_actors[k]] > 0)
                            neighbors++;

                        l++;
                    }

                    if (neighbors == 0)
                        winner_index = k;

                    l = 1;
                    neighbors = 0;
                    k++;
                }

//              If you didn't find an immediate winner, send a random neighbor to the opponent.
                if (winner_index == -1)
                {
                    int r;
                    srand (time(NULL));
                    if (adj_actors.size() == 1)
                        r = 0;
                    else
                        r = rand() % (adj_actors.size() - 1);

                    string actor = actor_string[adj_actors[r]];

//                  Set the connections for the chosen actor to 0 in the adjacency matrix.
                    for (int i = 1; i <= actors; i++)
                        adj_matrix[i][adj_actors[r]] = 0;

//                  Reset the vector with neighbors.
                    adj_actors.clear();

                    cout << actor << endl;
                }
//              If you did find an immediate winner, send it to the opponent and win the game.
                else
                {
                    string actor = actor_string[adj_actors[winner_index]];
                    cout << actor << endl;
                }
            }
        }
    }
}

//  The strategy used if you are playing as Veronique.
void strategyVeronique(int actors, int **adj_matrix, maptype actor_index, string actor_string[])
{
//  Start the game with sending a random actress to the opponent.
    srand(time(NULL));
    int r = rand() % actors + 1;

//  Set the connections for the chosen actress to 0 in the adjacency matrix.
    for (int i = actors + 1; i <= 2*actors; i++)
        adj_matrix[r][i] = 0;

    string actress = actor_string[r];
    cout << actress << endl;

    vector<int> adj_actresses;
    int winner_index = -1;

    while(true)
    {
        auto& input = cin;
        string actor;
        input >> actor;

        auto found_actor = actor_index.find(actor);
        int index = found_actor->second;

//      Find all neighbors of the given actor in a vector.
        for (int i = 1; i <= actors; i++)
        {
            if(adj_matrix[i][index] > 0)
            {
//              Check if one of the neighbors results in an immediate loss.
//              If yes, don't add it to the vector of neighbors.
                bool valid = true;
                int neighbors = 0;
                for (int l = actors + 1; l <= 2*actors; l++)
                {
                    if (l != index)
                    {
                        if (adj_matrix[i][l] > 0)
                        {
                            for (int k = 1; k <= actors; k++)
                            {
                                if (adj_matrix[k][l] > 0)
                                    neighbors++;
                            }

                            if (neighbors == 1)
                                valid = false;

                            neighbors = 0;
                        }
                    }
                }

                if (valid)
                    adj_actresses.push_back(i);
            }
        }

//      If no more neighbors exist you can't name another actress and you lose the game.
        if (adj_actresses.empty())
        {
            cout << "IGiveUp" << endl;
        }
        else
        {
//          Set the connections for the given actor to 0 in the adjacency matrix.
            for (int i = 1; i <= actors; i++)
                adj_matrix[i][index] = 0;

//          Check if one of the neighbors results in an immediate win and store its index.
            int k = 0;
            int l = actors + 1;
            int neighbors = 0;
            while (winner_index == -1 && k < adj_actresses.size())
            {
                while (neighbors == 0 && l <= 2*actors)
                {
                    if (adj_matrix[adj_actresses[k]][l] > 0)
                        neighbors++;

                    l++;
                }

                if (neighbors == 0)
                    winner_index = k;

                l = actors + 1;
                neighbors = 0;
                k++;
            }

//          If you didn't find an immediate winner, send a random neighbor to the opponent.
            if (winner_index == -1)
            {
                srand (time(NULL));
                if (adj_actresses.size() == 1)
                    r = 0;
                else
                    r = rand() % (adj_actresses.size() - 1);

                string actress = actor_string[adj_actresses[r]];

//              Set the connections for the chosen actress to 0 in the adjacency matrix.
                for (int i = actors + 1; i <= 2*actors; i++)
                    adj_matrix[adj_actresses[r]][i] = 0;

//              Reset the vector with neighbors.
                adj_actresses.clear();

                cout << actress << endl;
            }
//          If you did find an immediate winner, send it to the opponent and win the game.
            else
            {
                string actress = actor_string[adj_actresses[winner_index]];
                cout << actress << endl;
            }
        }
    }
}

int main (int argc, char** argv)
{
//  Map to store the indexes of the actresses/actors.
    maptype actor_index;

//    auto input = ifstream("samples\\m3.in");
    auto& input = cin;

    int actors, movies;
    input >> actors >> movies;

//  Array to store the names of the actresses/actors.
    string actor_string[2*actors + 1] = {};

//  Adjacency matrix to store the connections between actresses and actors.
    int **adj_matrix = new int *[2*actors + 1];
    for (int i = 0; i < 2*actors + 1; i++)
        adj_matrix[i] = new int[2*actors + 1];

//  Initialize the matrix to 0.
    for(int k = 1; k < 2*actors+1; k++)
    {
        for(int l = 1; l < 2*actors+1; l++)
        {
            adj_matrix[k][l] = 0;
        }
    }

//  Store all actresses/actors in the map with their corresponding index.
//  Store all names of actresses/actors in the array with the same index.
    for (int i = 1; i <= 2*actors; i++)
    {
        string act;
        input >> act;
        actor_index.insert(pair<string,int>(act, i));
        actor_string[i] = act;
    }

//  Run through all movies.
    for (int i = 0; i < movies; i++)
    {
        string movie;
        input >> movie;
        int n;
        input >> n;

//      Temporary arrays to separate the actresses and actors.
        int *actors_movie = new int[n];
        int *actresses_movie = new int[n];
//      Iterators for those arrays.
        int p = 0;
        int q = 0;

//      Run through the list of actresses/actors in the movie.
//      Store the actresses and actors in their corresponding array.
        for (int j = 0; j < n; j++)
        {
            string act;
            input >> act;
            auto found_actor = actor_index.find(act);
            if (found_actor->second < actors + 1)
            {
                actresses_movie[p] = found_actor->second;
                p++;
            } else
            {
                actors_movie[q] = found_actor->second;
                q++;
            }
        }

//      Add the connections from every actress to all actors in the adjacency matrix.
//      For every movie an actress and actor co-star in their connection get increased by one.
        if(p > 0 && q > 0)
        {
            for (int j = 0; j < p; j++)
            {
                for (int k = 0; k < q; k++)
                {
                    adj_matrix[actresses_movie[j]][actors_movie[k]]++;
                }
            }
        }

        delete[] actors_movie;
        delete[] actresses_movie;
    }

//  Get the name of the player and play the game in their perspective.
    string player;
    input >> player;

    if (player == "Mark")
        strategyMark(actors, adj_matrix, actor_index, actor_string);
    else
        strategyVeronique(actors, adj_matrix, actor_index, actor_string);

    return 0;
}
