#pragma once

#include "global.h"
#include "Imagine/Features.h"
#include <Imagine/Graphics.h>

// extract the descriptors of an image
void extract_descriptors(Imagine::Image<Imagine::Color, 2> I,
                         vector<Imagine::FVector<byte, 128> > &descriptors,
                         Imagine::Array<Imagine::SIFTDetector::Feature> &features, int accuracy);

// convert descriptors to Vector type
vector<Vector> convert_descriptors(const vector<Imagine::FVector<byte, 128> > &descriptors);

class Descriptor{
    int picture_id; // number of the image from which the descriptor has been extracted
    int centroid_id; // number of the nearest centroid
    Vector coordinates;
public:
    Descriptor(int i, Vector v); // initialize a descriptor with picture id i and coordinates v
    double operator[](int i); // accessor to coordinate i
    Vector get_coordinates(); // accessor to the vector of coordinates
    int get_centroid_id(); // accessor to the nearest centroid number
    int get_picture_id(); //accessor to the picture number
    void assign_centroid(int i); // set centroid_id to i
};

class DescriptorSet{
    int size; // set size
    int n_pictures; // number of pictures used
    vector<Descriptor> descriptors;
    vector<Vector> centroids;
    vector< vector<int> > clusters;
    vector< vector<int> > occurrences; // occurences[i][j] : number of occurrences of centroid i in picture j
public:
    DescriptorSet();
    Descriptor operator[](int i); // access to descriptor i
    int get_size(); // access to set size
    void add(Descriptor d); // add a descriptor to the set
    void add_image(Imagine::Image<Imagine::Color, 2> I, int picture_id, int accuracy); // extract the descriptors of an image and add them to the set
    void random_centroids(int k); // initialize k random centroids
    void compute_centroids(); // compute the centroid of each cluster
    void compute_clusters(); // find the nearest centroid for each descriptor and compute clusters
    void compute_occurrences(); // compute the occurrences of each centroid in each picture
    vector< vector<int> > get_clusters(); // access to the clusters
    vector<Vector> get_centroids(); // access to the centroids
    vector< vector<int> > get_occurrences(); // access to the occurrences
    void write_centroids(string s_centroids); // write the centroids in a .txt file
    void write_occurrences(string s_occurrences); // write the occurrences in a .txt file
    void read_centroids(string s_centroids); // read the centroids from a .txt file
    vector<int> clusters_size(); // compute the size of each cluster
    void kmeans(int k); // k-means
    void write_references(ref & references, string s_references);//write references
    ref read_references(string s_references);//read references
    void write_scores(string s_scores, ref & references,  vector<float> &scores);//write scores into ".txt" file
};

