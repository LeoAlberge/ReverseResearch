#include "descriptor.h"

// extract the descriptors of an image
void extract_descriptors(Imagine::Image<Imagine::Color, 2> I,
                         vector<Imagine::FVector<byte, 128> > &descriptors,
                         Imagine::Array<Imagine::SIFTDetector::Feature> &features, int accuracy){
    // detect feature points
    Imagine::SIFTDetector D;
    D.setFirstOctave(accuracy);
    features = D.run(I);

    // compute descriptors
    for(size_t i=0; i < features.size(); i++) {
        Imagine::SIFTDetector::Feature f = features[i];
        descriptors.push_back(f.desc);
    }
}

// convert descriptors to Vector type
vector<Vector> convert_descriptors(const vector<Imagine::FVector<byte, 128> > &descriptors){
    vector<Vector> converted_descriptors;
    for(int i = 0; i < descriptors.size(); i++){
        Vector converted_descriptor;
        for(int j = 0; j < 128; j++){
            int coord_j = descriptors[i][j];
            converted_descriptor[j] = coord_j;
        }
        converted_descriptors.push_back(converted_descriptor);
    }
    return converted_descriptors;
}

// Descriptor

// initialize a descriptor with picture id i and coordinates v
Descriptor::Descriptor(int i, Vector v){
    picture_id = i;
    coordinates = v;
}

// accessor to coordinate i
double Descriptor::operator[](int i){
    return coordinates[i];
}

// access to the vector of coordinates
Vector Descriptor::get_coordinates(){
    return coordinates;
}

// access to the nearest centroid number
int Descriptor::get_centroid_id(){
    return centroid_id;
}

// access to the picture number
int Descriptor::get_picture_id(){
    return picture_id;
}

// set centroid_it to i
void Descriptor::assign_centroid(int i){
    centroid_id = i;
}

// DescriptorSet

DescriptorSet::DescriptorSet(){
    size = 0;
    n_pictures = 0;
}

// access to descriptor i
Descriptor DescriptorSet::operator[](int i){
    return descriptors[i];
}

// access to set size
int DescriptorSet::get_size(){
    return size;
}

// add a descriptor to the set
void DescriptorSet::add(Descriptor d){
    descriptors.push_back(d);
    size++;
}

// extract the descriptors of an image and add them to the set
void DescriptorSet::add_image(Imagine::Image<Imagine::Color, 2> I, int picture_id, int accuracy){
    vector<Imagine::FVector<byte, 128> > descriptors;
    Imagine::Array<Imagine::SIFTDetector::Feature> features;
    extract_descriptors(I, descriptors, features,accuracy);
    vector<Vector> v = convert_descriptors(descriptors);
    vector<Vector>::iterator it;
    for(it = v.begin(); it != v.end(); ++it){
        Descriptor d(picture_id,(*it));
        this->add(d);
    }
    n_pictures++;
}

// initialize k random centroids
void DescriptorSet::random_centroids(int k){
    init_random();
    vector<int> index_tab;

    // add k distinct centroids
    while(index_tab.size() != k){
        int index = random_int(0, size);
        bool in_tab = false;
        // draw an index and check if it is already in tab
        for(int i = 0; i < index_tab.size(); i++){
            if(index == index_tab[i]){
                in_tab = true;
            }
        }
        // if not already in tab, add it
        if(!in_tab){
            index_tab.push_back(index);
            Vector centroid = descriptors[index].get_coordinates();
            centroids.push_back(centroid);
        }
    }
}

// compute the centroid of each cluster
void DescriptorSet::compute_centroids(){
    vector<Vector> new_centroids;

    for(int i = 0; i < clusters.size(); i++){
        // compute the centroid of cluster i
        Vector centroid_i(v_size);
        for(int j = 0; j < v_size; j++){
            // initialize coordinate to 0
            centroid_i[j] = 0;
            // compute coordinate j
            for(int k = 0; k < clusters[i].size(); k++){
                int vect_index = clusters[i][k];
                Vector descriptor = descriptors[vect_index].get_coordinates();
                centroid_i[j] += descriptor[j];
            }
            centroid_i[j] *= 1. / (clusters[i].size());
        }
        // add centroid i to the list
        new_centroids.push_back(centroid_i);
    }
    // replace old centroids
    centroids = new_centroids;
}

// find the nearest centroid for each descriptor and compute clusters
void DescriptorSet::compute_clusters(){
    vector< vector<int> > new_clusters(centroids.size());

    for(int i = 0; i < size; i++){
        // find the nearest centroid from descriptor i
        Vector descriptor_i = (descriptors[i]).get_coordinates();

        int nearest_centroid_index = 0;
        Vector nearest_centroid = centroids[0];

        for(int j = 0; j < centroids.size(); j++){
            Vector centroid_j = centroids[j];
            if(dist(descriptor_i, centroid_j) < dist(descriptor_i, nearest_centroid)){
                nearest_centroid_index = j;
                nearest_centroid = centroids[j];
            }
        }
        // add the descriptor to the cluster with centroid nearest_centroid_index
        new_clusters[nearest_centroid_index].push_back(i);
        descriptors[i].assign_centroid(nearest_centroid_index);
    }
    clusters = new_clusters;
}

// compute the occurrences of each centroid in each picture
void DescriptorSet::compute_occurrences(){
    vector< vector<int> > occurrences_tab;
    for(int i = 0; i < centroids.size(); i++){
        vector<int> occurrences_i(n_pictures, 0);
        occurrences_tab.push_back(occurrences_i);
    }
    for(int i = 0; i < size; i++){
        int centroid_id = descriptors[i].get_centroid_id();
        int picture_id = descriptors[i].get_picture_id();
        occurrences_tab[centroid_id][picture_id]++;
    }
    occurrences = occurrences_tab;
}

// access to the clusters
vector< vector<int> > DescriptorSet::get_clusters(){
    return clusters;
}

// access to the centroids
vector<Vector> DescriptorSet::get_centroids(){
    return centroids;
}

// access to the occurrences
vector< vector<int> > DescriptorSet::get_occurrences(){
    return occurrences;
}

// write the centroids in a .txt file
void DescriptorSet::write_centroids(string s_centroids){
    ofstream file(s_centroids + ".txt", ios::out | ios::trunc);  // open file
    if(file){ // if opening succeed
        for(int i = 0; i < centroids.size(); i++){
            // write centroid i
            for(int j = 0; j < 128; j++){
                file << centroids[i][j] << ",";
            }
            file << endl;
        }
        file.close();
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
}

void DescriptorSet::write_references(std::map<int,string> & references, string s_references){
    ofstream file(s_references + ".txt",ios::out|ios::trunc); //open file
    std::map<int,string>::iterator it;
    if(file){ // if opening succeed
        for(it = references.begin();it!= references.end();++it){
            // write reference
            file << (*it).first << ","<<(*it).second;
            file << endl;
        }
        file.close();
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
}


// write the occurrences in a .txt file
void DescriptorSet::write_occurrences(string s_occurrences){
    ofstream file(s_occurrences + ".txt", ios::out | ios::trunc);  // open file
    if(file){ // if opening succeed
        for(int i = 0; i < occurrences.size(); i++){
            // write occurrences of centroid i
            for(int j = 0; j < occurrences[i].size(); j++){
                file << occurrences[i][j] << ",";
            }
            file << endl;
        }
        file.close();
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
}

// read the centroids from a .txt file
void DescriptorSet::read_centroids(string s_centroids){
    ifstream file(s_centroids + ".txt", ios::in); // open file
    if(file){ // if opening succeed
        string str;
        while(getline(file, str)){ // read a line (the coordinates of a centroid)
            int i = 0;
            int j = 0;
            Vector centroid;
            while(i < str.size()){
                string sub_str;
                while(str[i] != ','){
                    sub_str += str[i];
                    i++;
                }
                centroid[j] = stof(sub_str);
                j++;
                i++;
            }
            centroids.push_back(centroid);
        }
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
}

// compute the size of each cluster
vector<int> DescriptorSet::clusters_size(){
    vector<int> sizes;
    for(int i = 0; i < clusters.size(); i++){
        sizes.push_back(clusters[i].size());
    }
    return sizes;
}

// k-means
void DescriptorSet::kmeans(int k){
    assert(k > 0 && size > 0 && k <= size);

    this->random_centroids(k);
    this->compute_clusters();

    // convergence variable
    bool stop = false;

    while(!stop){

        this->compute_centroids();

        // store old clusters
        vector< vector<int> > old_clusters = clusters;

        // compute new clusters
        this->compute_clusters();

        // test convergence
        stop = (clusters == old_clusters);
    }
}

ref  DescriptorSet::read_references(string s_references){
    ref references;
    ifstream file(s_references + ".txt", ios::in); // open file
    if(file){ // if opening succeed
        string str;
        while(getline(file, str)){ // read a line (the coordinates of a centroid)
            //cout<<str<<endl;
            int i = 0;
            int j=0;
            string sub_str;
            string sub_str2;
            while(str[i] != ','){
                sub_str += str[i];
                i++;
            }
            i++;
            while(i<str.size()){
                sub_str2 += str[i];
                j++;
                i++;
            }
            //cout<<sub_str<<"---"<<sub_str2<<endl;
            references[std::stoi(sub_str)]=sub_str2;
        }
        return references;
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
}

void DescriptorSet::write_scores(string s_scores, ref &references, vector<float> & scores){
    ofstream file(s_scores + ".txt", ios::out | ios::trunc);  // open file
    if(file){ // if opening succeed
        for(int i=0;i<scores.size();i++){
            file << references[i] <<","<<scores[i]<<endl;
        }
        file.close();
    }
    else{
        std::cout << "Error while opening file!" << endl;
    }
}
