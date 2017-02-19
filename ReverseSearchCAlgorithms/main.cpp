#define _USE_MATH_DEFINES
#include <cmath>
#include<vector>
#include<string>
#include<map>
#include <dirent.h>

using namespace std;

#include<Imagine/Graphics.h>

#include "descriptor.h"
#include "global.h"
#include "tools.h"

// processing of the database of examples
void process_db_example(const int size, int k, string s_centroids, string s_occurrences, int accuracy){
    cout << "Database processing ..." << endl;

    DescriptorSet set;

    cout << "Extracting descriptors ..." << endl;

    for(int i = 0; i < size; i++){
        string path = srcPath("Images/");
        path += to_string(i) + ".jpg";
        Imagine::Image<Imagine::Color, 2> I;
        bool b = Imagine::load(I, path);
        set.add_image(I, i, accuracy);
    }

    cout << "Number of descriptors : " << set.get_size() << endl;

    // k-means
    cout << "K-means ..." << endl;
    set.kmeans(k);

    // write centroids in .txt file
    set.write_centroids(s_centroids);

    // compute the occurrences and write them in a .txt file
    set.compute_occurrences();
    set.write_occurrences(s_occurrences);

    cout << "Database processed" << endl;
    cout << endl;
}

// processing of the database
void process_db( int k, string s_centroids, string s_occurrences,string s_references,  string path_dir, int accuracy){
    cout << "Database processing ..." << endl;

    DescriptorSet set;
    int size=0;
    int i=0;
    std::map<int,string> references;
    const char * dir = path_dir.data();


    cout << "Extracting descriptors ..." << endl;


    DIR * rep = opendir(dir);
    if (rep != NULL)
    {
        struct dirent * ent;

        while ((ent = readdir(rep)) != NULL)
        {
            //We read image in the directory : there must be only JPEG images
            if( i>1){

                string path =path_dir;
                path+=ent->d_name;
                Imagine::Image<Imagine::Color, 2> I;
                bool b = Imagine::load(I, path);
                if(!b)
                    std::cout<<"Image loading error"<<endl;
                //On ajoute ces descriptors au Set avec son identifiant ici taille
                //On ajoute la reference
                references[size]=path;
                set.add_image(I, size,accuracy);
                size++;

            }
            i++;
        }
        closedir(rep);
    }

    cout << "Number of descriptors : " << set.get_size() << endl;

    // k-means
    cout << "K-means ..." << endl;
    set.kmeans(k);

    // write centroids in .txt file
    set.write_centroids(s_centroids);

    // compute the occurrences and write them in a .txt file
    set.compute_occurrences();
    set.write_occurrences(s_occurrences);
    set.write_references(references,s_references);

    cout << "Database processed" << endl;
    cout << endl;
}

// search request example
vector<float> request_example(const int size, string s_request, string s_centroids, string s_occurrences, int accuracy){
    cout << "Request processing..." << endl;
    cout << "Extracting descriptors..." << endl;

    // extract descriptors of the requested image
    string path = srcPath("Images/" + s_request + ".jpg");
    Imagine::Image<Imagine::Color, 2> I;
    bool b = Imagine::load(I, path);

    DescriptorSet set_request;
    set_request.add_image(I, 1, accuracy);

    cout << "Number of descriptors : " << set_request.get_size() << endl;

    // read centroids from .txt file
    set_request.read_centroids(s_centroids);

    // compute clusters
    set_request.compute_clusters();
    vector< vector<int> > request_clusters = set_request.get_clusters();

    // read occurrences from .txt file
    vector< vector<int> > db_occurrences = read_occurrences(size, s_occurrences);

    // compute the total number of occurrences for each centroid
    // and the number of centroids used in each image
    vector<int> centroids_occurrences(request_clusters.size(), 0);
    vector<int> centroids_in_image(size, 0);
    compute_total_occurrences(db_occurrences, centroids_occurrences, centroids_in_image);

    // compute the score of each image
    vector<float> scores;

    for(int i = 0; i < size; i++){
        scores.push_back(score(i, size, request_clusters, centroids_occurrences, centroids_in_image, db_occurrences));
    }

    cout << "Request processed" << endl;
    cout << endl;

    return scores;
}

// search request example
void request( string s_request, string s_centroids, string s_occurrences, string s_references, string s_scores, int accuracy){
    cout << "Request processing..." << endl;
    cout << "Extracting descriptors..." << endl;

    // extract descriptors of the requested image
    string path = s_request;
    Imagine::Image<Imagine::Color, 2> I;
    bool b = Imagine::load(I, path);

    DescriptorSet set_request;
    set_request.add_image(I, 1, accuracy);

    std::map<int,string> references=set_request.read_references(s_references);
    std::map<int,string>::iterator it;
    int size=0;
    for(it=references.begin();it!=references.end();++it){
        size++;
        cout<<(*it).first<<"---"<<(*it).second<<endl;
    }

    cout << "Number of descriptors : " << set_request.get_size() << endl;

    // read centroids from .txt file
    set_request.read_centroids(s_centroids);

    // compute clusters
    set_request.compute_clusters();
    vector< vector<int> > request_clusters = set_request.get_clusters();

    // read occurrences from .txt file
    vector< vector<int> > db_occurrences = read_occurrences(size, s_occurrences);

    // compute the total number of occurrences for each centroid
    // and the number of centroids used in each image
    vector<int> centroids_occurrences(request_clusters.size(), 0);
    vector<int> centroids_in_image(size, 0);
    compute_total_occurrences(db_occurrences, centroids_occurrences, centroids_in_image);

    // compute the score of each image
    vector<float> scores;

    for(int i = 0; i < size; i++){
        scores.push_back(score(i, size, request_clusters, centroids_occurrences, centroids_in_image, db_occurrences));
    }
    cout << "Writing scores" << endl;
    set_request.write_scores(s_scores,references,scores);


    cout << "Request processed" << endl;
    cout << endl;
}

int main(int argc, char *argv[]){

    const int size = 10;
    int k = 100;
    int accuracy_example=0;
    string s_request2 = argv[1];
    int accuracy = std::stoi(argv[2]);
    //string dir =argv[1];


    string s_request = "1";
    string s_occurences = "occurrences";
    string s_references="references";
    string s_scores="scores";
    string s_centroids = "centroids";

//     process_db_example(size, k, s_centroids, s_occurences, accuracy_example);
//     vector<float> scores = request_example(size, s_request, s_centroids, s_occurences,accuracy_example);
//     for(int i=0; i<scores.size();i++){
//         cout<<"Image : "<<i<< "- Scores : "<<scores[i]<<endl;
//     }

    //process_db(k,s_centroids,s_occurences,s_references,dir,accuracy);
    request( s_request2, s_centroids, s_occurences, s_references, s_scores,accuracy);


    return 0;
}
