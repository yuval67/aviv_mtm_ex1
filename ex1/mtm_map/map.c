#include "stdio.h"
 #include "stdlib.h"
 #include "./map.h"
 #define EMPTY_MAP -1

typedef struct Node_t
{
    MapKeyElement key;
    MapDataElement value;
    struct Node_t *next; 
} *Node;


struct Map_t
{
    Node head;
    Node iterrator; 
    freeMapDataElements func_free_map_data_element;
    freeMapKeyElements func_free_key_data_element;
    copyMapDataElements func_copy_map_data_element;
    copyMapKeyElements func_copy_map_key_element;
    compareMapKeyElements func_compare_map_key_element;
};


//Additional internal function: the perpose of the function is creating one singular node in order to put it in the map later on
static Node MapcreateNewNode (Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if (map == NULL || keyElement == NULL || dataElement == NULL){
        return NULL;
    }
    Node temp = (Node) malloc(sizeof(struct Node_t));
    if (temp == NULL){
        return NULL;
    }
    temp->key = map->func_copy_map_key_element(keyElement);
    temp->value = map->func_copy_map_data_element(dataElement);
    temp->next = NULL;
    return temp;
}


Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if (copyDataElement == NULL || copyKeyElement == NULL || freeDataElement == NULL ||
     freeKeyElement == NULL || compareKeyElements == NULL)
    {
        return NULL;
    }
    Map new_map = (Map)malloc(sizeof(struct Map_t));
    if (new_map == NULL)
    {
        return NULL;
    }
    new_map->head = NULL;
    new_map->func_free_map_data_element = freeDataElement;
    new_map->func_free_key_data_element = freeKeyElement;
    new_map->func_copy_map_key_element = copyKeyElement;
    new_map->func_copy_map_data_element = copyDataElement;
    new_map->func_compare_map_key_element = compareKeyElements;
    new_map->iterrator = NULL;
    return new_map;
}


int mapGetSize (Map map)
{
    if (map == NULL)
    {
        return EMPTY_MAP;
    }
    int size = 0;
    Node ptr = map->head;
    while (ptr != NULL)
    {
        size++;
        ptr = ptr->next;
    }
    return size;
}

MapResult insertInMiddle (Map map, MapKeyElement keyElement,MapDataElement dataElement , Node ptr_prev, Node ptr)
{
    Node new_node=MapcreateNewNode(map, keyElement,dataElement);
    if (new_node==NULL){
        return MAP_OUT_OF_MEMORY;
    }
    if (ptr!=map->head){
        ptr_prev->next=new_node;
        new_node->next=ptr;
        map->iterrator=NULL;
        return MAP_SUCCESS;
    }
    else{
        new_node->next=map->head;
        map->head=new_node;
        map->iterrator=NULL;
        return MAP_SUCCESS;
    }
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if (map == NULL || keyElement == NULL || dataElement == NULL){
            return MAP_NULL_ARGUMENT;
    }
    map->iterrator=NULL;
    if (map->head==NULL){
            Node new_node=MapcreateNewNode(map, keyElement,dataElement);
        if (new_node==NULL){
            return MAP_OUT_OF_MEMORY;
            }

        map->head=new_node;
        return MAP_SUCCESS;
    }
    Node ptr= map->head;
    Node ptr_prev=map->head;
    while (ptr->next!=NULL)
    {
        if (map->func_compare_map_key_element(ptr->key,keyElement)==0)
        {
            ptr->value=map->func_copy_map_data_element(dataElement);
            return MAP_SUCCESS;
        }
        if (map->func_compare_map_key_element(ptr->key,keyElement)<0)//
        {
            return insertInMiddle(map,keyElement,dataElement,ptr_prev,ptr);
        }
        ptr_prev=ptr;
        ptr=ptr->next;
    }
    //got to last place:
    if (map->func_compare_map_key_element(ptr->key,keyElement)==0)
        {
            ptr->value=map->func_copy_map_data_element(dataElement);
            return MAP_SUCCESS;
        }
    Node new_node=MapcreateNewNode(map, keyElement,dataElement);
    if (new_node==NULL){
        return MAP_OUT_OF_MEMORY;
    }
    if (map->func_compare_map_key_element(ptr->key,keyElement)<0 && ptr!=map->head && ptr->next==NULL){
        ptr_prev->next=new_node;
        new_node->next=ptr;
        return MAP_SUCCESS;
        }
    if (ptr==map->head && map->func_compare_map_key_element(ptr->key,keyElement)<0 && ptr->next==NULL)
    {
        map->head=new_node;
        map->head->next=ptr;
        return MAP_SUCCESS;
    }
    //ptr->next==NULL && map->func_compare_map_key_element(ptr->key,keyElement)>0)
    ptr->next=new_node;
    return MAP_SUCCESS;
}




//the function
// MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
//     if (map == NULL || keyElement == NULL || dataElement == NULL){
//         return MAP_NULL_ARGUMENT;
//     }
//     Node new_node = MapcreateNewNode (map, keyElement, dataElement);
//     if (new_node == NULL){
//         return MAP_OUT_OF_MEMORY;
//     }
//     if (map->head == NULL){
//         map->head = new_node;
//         map->iterrator = NULL;
//         return MAP_SUCCESS;
//     }
//     Node i = map->head;
//     if (ptr == NULL){
//         return MAP_OUT_OF_MEMORY;
//     }
//     while (ptr->next != NULL && map->func_compare_map_key_element(ptr->key,keyElement) < 0){ 
//         ptr = ptr->next;
//     }
//     if(ptr->next == NULL){
//        if(map->func_compare_map_key_element(ptr->key,keyElement) < 0){
//             ptr->next = new_node;
//             new_node->next = NULL;
//             return MAP_SUCCESS;         
//         }
//         if(map->func_compare_map_key_element(ptr->key,keyElement) == 0){
//             ptr->value=map->func_copy_map_data_element(dataElement);
//             map->iterrator=NULL;
//             return MAP_SUCCESS;        
//         }
//         else{
//         new_node->next = ptr;
//         ptr->next = NULL;       
//         return MAP_SUCCESS;
//         }
//     }
//     if(map->func_compare_map_key_element(ptr->key,keyElement) == 0)
//     {
//         ptr->value=map->func_copy_map_data_element(dataElement);
//         map->iterrator=NULL;
//         return MAP_SUCCESS;
//     }
//     new_node->next = ptr->next;
//     ptr->next = new_node;
//     return MAP_SUCCESS;
// }

//end function --------------------------------------------------------------------------------------------


Map mapCopy(Map map){
    if (map == NULL){
        return NULL;
    }
    map->iterrator = NULL;
    Map copied_map = mapCreate (map-> func_copy_map_data_element,
                                map-> func_copy_map_key_element,
                                map-> func_free_map_data_element,
                                map-> func_free_key_data_element,                             
                                map-> func_compare_map_key_element);
    Node ptr = map->head;
    while(ptr != NULL){
 		if(mapPut(copied_map, ptr->key, ptr->value) != MAP_SUCCESS){
            mapDestroy(copied_map);
            return NULL;
        }
        ptr = ptr->next;
    }
    return copied_map;  
}



//Additional internal function: the perpose is to remove the head.
//Returns MAP_NULL_ARGUMENT when the map is NULL.
//Returns MAP_SUCCESS when head removes or head is NULL
static MapResult mapRemoveHead(Map map)
{
   if (map == NULL)
   {
       return MAP_NULL_ARGUMENT;
   }
   if(map->head == NULL ){ //I think that needs to return "MUP_NULL_ARGUMENT" as well
       return MAP_SUCCESS; // because the map is emmpty I we did not do anything do the map is null in some way
   }
   map->iterrator = NULL; 
   Node first = map->head;
   map->head = map->head->next;
   if (first->key != NULL){
       map->func_free_key_data_element(first->key);
   }
   if (first->value != NULL){
       map->func_free_map_data_element(first->value);
   }
   free(first);
   return MAP_SUCCESS;
}

bool mapContains(Map map, MapKeyElement element){
    if (map == NULL || element == NULL){
        return false;
    }
    Node ptr = map->head;
    // if (ptr == NULL){
    //     return false;
    // }
    while((ptr != NULL && map->func_compare_map_key_element(ptr->key, element)) != 0){
        ptr = ptr-> next;
    }
    if (ptr == NULL){
        return false;
    }
    return true;
}


//always need to return the heat to it's origen- in this function you lose it
//does not contine the MAP_ITEM_DOES_NOT_EXIST return option
// I wrote my own function after
MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    if (map == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    map->iterrator = NULL;
    if (!mapContains(map, keyElement)){
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    Node ptr = map->head; //try from here
//     if ((map->func_compare_map_key_element(ptr->key, keyElement)) == 0){//when the first node is the one we need to remove

//         map->head = map->head->next;
//         map->func_free_key_data_element(ptr->key);
//         map->func_free_map_data_element(ptr->value);
//         free(ptr);
//         return MAP_SUCCESS;
//     }
//     ptr = ptr->next;
//     while ((map->func_compare_map_key_element(ptr->key, keyElement))!= 0 && (ptr->next != NULL))
//   {
//       map->iterrator = ptr;
//       ptr = ptr->next;
//   }
//   if ((map->func_compare_map_key_element(ptr->key, keyElement)) == 0){
//         (map->iterrator)->next = (ptr->next);
//         map->func_free_map_data_element (ptr->value);
//         map->func_free_key_data_element (ptr->key);
//         free(ptr);
//         map->iterrator = NULL;
//         return MAP_SUCCESS;
//   }
//   return MAP_ITEM_DOES_NOT_EXIST;
// }
//to here ------------------------------
    ptr = ptr->next;
    Node ptr_previous = map->head;
    if(map->func_compare_map_key_element(ptr_previous->key, keyElement) == 0){
        return mapRemoveHead(map);
    }
    while((ptr != NULL) && (map->func_compare_map_key_element(ptr->key, keyElement) !=0)){
        ptr = ptr->next;
        ptr_previous = ptr_previous->next;
    } 
    if(ptr == NULL){
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    map->func_free_map_data_element(ptr->value);
    map->func_free_key_data_element (ptr->key);
    ptr_previous->next = ptr->next;
    free(ptr);
    return MAP_SUCCESS;
}


//my fanction of mapRemove
/*yuval: i did the function for two situation:
1. if the arg i want to remove if the head
2. its not the head
yoe did not sperate the two options, so cnly remove the head.
*/
///MapResult mapRemove(Map map, MapKeyElement keyElement)
///{
///    if (map == NULL || keyElement == NULL)
///    {
///        return MAP_NULL_ARGUMENT;
///    }
///    if (!mapContains(map, keyElement)){ //great!!
///        return MAP_ITEM_DOES_NOT_EXIST;
///    }
///    Node head_keepr=map->head;
///    Node previos_node=NULL;
///    if (map->func_compare_map_key_element(map->head->key, keyElement)==0)
///    {
///        map->head=map->head->next; // yuval:loaking memory we need to release the space by using the head keeper
///        return MAP_SUCCESS;
///    }
///    previos_node=map->head;
///    map->head=map->head->next;
///    while (map->head!=NULL)
///    {
///        if(map->func_compare_map_key_element(map->head->key, keyElement)==0)
///        {
///            map->func_free_map_data_element(map->head->value);
///            map->func_free_key_data_element(map->head->key);
///            map->head=map->head->next;
///            free (previos_node->next);
///            previos_node->next=map->head;
///            map->head=head_keepr;
///            return MAP_SUCCESS;
///        }
///        previos_node=map->head;
///        map->head=map->head->next;
///    }
///    return MAP_SUCCESS; //doen not matter because has to exist
///}
///

MapResult mapClear(Map map){
    if (map == NULL){
        return MAP_NULL_ARGUMENT;
    }
    while (map->head !=NULL){
        mapRemoveHead(map);
    }
    map->iterrator = NULL;
    return MAP_SUCCESS;
}


void mapDestroy(Map map)
{
    if (map == NULL){
        return;
    }
    while (map->head != NULL) {
        mapRemoveHead(map);
    }
    free(map->head);
    free(map);
    return;
}


//assistant with github
//distroyes all the map-everything- keys & values.
// I think is better
//yuval: mine is more simple and we already have clearmap fun, why wont we use this?

///void mapDestroy(Map map)
///{
///    if (map == NULL){
///        return;
///    }
///    Node current_node= map->head;
///    Node next_node;
///    while (current_node!=NULL)
///    {
///        next_node=current_node->next;
///        map->func_free_key_data_element(current_node->key);
///        map->func_free_map_data_element(current_node->value);
///        free(current_node);
///        current_node=next_node;
///    }
///    current_node=NULL; //yuval:why null- need to be free i think
///    next_node=NULL; //yuval:same
///    free(map);
///    map=NULL;
///}


//there is a way to make more affienait- but i do not know how yet yuval: does not matter!
//do not neet to use mapPut because we copy in the same order , yuval: i dont understand, put is for insert we need it

MapKeyElement mapGetFirst(Map map)
{
    if (map == NULL || map->head == NULL)
    {
        return NULL;
    }
    if (map->iterrator == NULL){ 
        return NULL;
    }
    map->iterrator = map->head;
    Node copied_key = MapcreateNewNode(map, map->iterrator->key, map->iterrator->value);
    if(copied_key == NULL){
        return NULL;
    }
    return copied_key->key;
}

// MapKeyElement mapGetFirst(Map map)
// {
//     if (map==NULL || map->head==NULL)
//     {
//         return NULL;
//     }
//     if (map->iterrator == NULL){ 
//         return NULL;
//     }
//     map->iterrator = map->head;
//     return map->head->key;
// }

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if (map == NULL || keyElement == NULL){
        return NULL; 
    }
    Node ptr = map->head;
    // if (ptr == NULL)
    // {
    //     return NULL;
    // }
    while((ptr->next) != NULL){ 
        if (map->func_compare_map_key_element(ptr->key, keyElement) == 0){
            return ptr->value;
        }
        ptr= ptr->next;
    }
    if (map->func_compare_map_key_element(ptr->key, keyElement) == 0){
        return ptr->value;
    }
    return NULL; 
}

MapKeyElement mapGetNext(Map map) { 
    if ((map->iterrator->next == NULL || map->iterrator == NULL) || map == NULL){
        return NULL;
    }
    map->iterrator = map->iterrator->next;
    MapKeyElement result = map->iterrator->key;
    return result;
    //return (map->iterrator-> key);
    Node copied_key = MapcreateNewNode(map, map->iterrator->key, map->iterrator->value);
    if(copied_key == NULL){
        return NULL;
    }
    return copied_key->key;
}


// MapKeyElement mapGetNext(Map map) { 
//     if ((map->iterrator->next == NULL || map->iterrator == NULL) || map == NULL){
//         return NULL;
//     }
//     map->iterrator = map->iterrator->next;
//     MapKeyElement result = map->iterrator->key;
//     return result;
//     //return (map->iterrator-> key);
// }