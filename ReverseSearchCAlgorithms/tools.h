#pragma once

#include <iostream>
using namespace std;

// compute the cosinus of two vectors
float cosinus (vector<float> u, vector<float> v){
    float scal_prod = 0;
    float norm_u = 0;
    float norm_v = 0;
    for(int i = 0; i < u.size(); i++){
        scal_prod += u[i] * v[i];
        norm_u += u[i] * u[i];
        norm_v += v[i] * v[i];
    }
    return scal_prod / (sqrt(norm_u) * sqrt(norm_v));
}

// read the occurrences from a .txt file
vector< vector<int> > read_occurrences(int size, string s_occurrences){
    vector < vector<int> > occurrences;
    ifstream file(s_occurrences + ".txt", ios::in); // open file
    if(file){ // if opening succeed
        string str;
        while(getline(file, str)){ // read a line (the occurrences of a centroid)
            int i = 0;
            int j = 0;
            vector<int> occurrence(size, 0);
            while(i < str.size()){
                string sub_str;
                while(str[i] != ','){
                    sub_str += str[i];
                    i++;
                }
                occurrence[j] = stof(sub_str);
                j++;
                i++;
            }
            occurrences.push_back(occurrence);
        }
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
    return occurrences;
}

// compute the total number of occurrences of each centroid in the database
// compute the total number of centroids used in each image
void compute_total_occurrences(vector< vector<int> > db_occurrences,
                               vector<int> &centroids_occurrences, vector<int> &centroids_in_image){
    for(int i = 0; i < db_occurrences.size(); i++){ // for all centroids
        for(int j = 0; j < db_occurrences[i].size(); j++){ // for all images
            centroids_occurrences[i] += db_occurrences[i][j];
            if(db_occurrences[i][j] > 0)
                centroids_in_image[j]++;
        }
    }
}

// compute the similarity score of image i with respect to the request image
float score(int i, int size, vector< vector<int> > request_clusters, vector<int> centroids_occurrences,
            vector<int> centroids_in_image, vector< vector<int> > db_occurrences){
    int n_centroids = db_occurrences.size();
    int centroids_used_request = 0;
    vector<float> freq_i(n_centroids, 0);
    vector<float> freq_request(n_centroids, 0);
    for(int j = 0; j < n_centroids; j++){
        if(request_clusters[j].size() > 0)
            centroids_used_request ++;
    }
    for(int j = 0; j < n_centroids; j++){
        freq_i[j] = ((float)db_occurrences[j][i] / (float)centroids_in_image[i]) * log((float)size / (float)centroids_occurrences[j]);
        freq_request[j] = ((float)request_clusters[j].size() / (float)centroids_used_request) * log((float)size / (float)centroids_occurrences[j]);
    }
    return cosinus(freq_i, freq_request);
}

