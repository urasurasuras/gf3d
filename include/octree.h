// https://github.com/block8437/buildaverse
#ifndef _OCTREE_H
#define _OCTREE_H

#include <stdbool.h>

// Maximum amount of children before a subdivision
#define OCTREE_MAX_CHILDREN 4

// Initial size and growth rate of result data
#define OCTREE_RESULT_SIZE 16

// Data structure for 3D objects for octrees
typedef struct {
    // Position
    float x;
    float y;
    float z;

    // Dimmensions
    float width;
    float height;
    float depth;
} octree_obj_t;

// Data structure for holding data in octrees
typedef struct {
    octree_obj_t obj;
    void* data;
} octree_data_t;

// Data structure for octree nodes
struct octree_node_s;
typedef struct octree_node_s octree_node_t;

struct octree_node_s {
    octree_obj_t obj;

    octree_node_t* leafs[8];
    octree_data_t* children[OCTREE_MAX_CHILDREN];
};

// Data structure for general octrees
typedef struct {
    octree_node_t* root;
} octree_t;

// Data structure for holding results of an octree query
typedef struct {
    octree_data_t** data; // Growing data array

    unsigned int index; // Head of the array
    unsigned int size; // Size of current allocation
} octree_results_t;

// Allocate and setup an octree
// Return allocated obj, and NULL on failure
octree_t* octree_init(float width, float height, float depth);

// Insert an object into the octree
// Return true on success
bool octree_add(octree_t* octree, void* ptr, void * ent);

// Search an octree for a specific boundry
// Return results object containing objects intersecting the boundary on success,
// NULL on failure.
octree_results_t* octree_search(octree_t* octree, octree_obj_t query);

// Release all memory of octree root
void octree_clear(octree_node_t * root);

// Release all memory allocated by an octree
void octree_free(octree_t* octree);

#endif
