/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

#include <string.h> // strcmp

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
int init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO

    c->size = 0;

    //inicialise on empty cluster
    if(cap == 0)
    {
        c->capacity = 0;
        c->obj = NULL;
        return 0;
    }

    c->obj = malloc(cap * sizeof(struct obj_t));
    if(c->obj == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return -1;
    }
    c->capacity = cap;
    return 0;

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO

    free(c->obj);
    //inicialise on empty cluster
    init_cluster(c, 0);
}


//clear all clusters
void clear_clusters(struct cluster_t *c, int n)
{
    assert(c);
    
    for(int i = 0; i < n; i++)
    {
        clear_cluster(&c[i]);
    }
    free(c);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    assert(c->obj);

    // TODO

    // resize cluster if needed
    if (c->size >= c->capacity)
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    // add object to the last "shelf" on cluster    
    c->obj[c->size] = obj;
    // we added one object to cluster
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO

    // append all objects from c2 to c1
    for (int i = 0; i < c2->size; i++)
        append_cluster(c1, c2->obj[i]);
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO

    clear_cluster(&carr[idx]);
    // move all clusters after idx to idx
    for (int i = idx; i < narr - 1; i++)
        carr[i] = carr[i + 1];
    // decrease number of clusters by one   
    narr--;
    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO

    float euklid_dist = 0.0;
    euklid_dist = sqrt((pow(o2->x-o1->x, 2)) + (pow(o2->y-o1->y, 2)));
    return euklid_dist;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2, int switcher)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO

    float min_max_dist;
    if (switcher == 0)
    // maximum possible distance between two points +1 
    // (length of the field diagonal)
        min_max_dist = 1415;
    if (switcher == 1)
    //minimum possible distance between two points -1
        min_max_dist = -1;
    float current_dist;
    for(int i = 0; i < c1->size; i++)
    {
        for(int j = 0; j < c2->size; j++)
        {
            current_dist = obj_distance(&c1->obj[i], &c2->obj[j]);
            // find minimum distance through all objects in both clusters
            if (switcher == 0)
            {
                if (current_dist < min_max_dist)
                    min_max_dist = current_dist;
            }
            // find maximum distance through all objects in both clusters
            if (switcher == 1)
            {
                if (current_dist > min_max_dist)
                    min_max_dist = current_dist;
            }
            // if (current_dist < min_max_dist)
                // min_dist = current_dist;
        }
    }

    return min_max_dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2, int switcher)
{
    assert(narr > 0);

    // TODO

    float shortest_dist = -1;
    float current_dist;

    for (int i = 0; i < narr; i++) 
    {
        for (int j = i + 1; j < narr; j++)
        {   
            current_dist = cluster_distance(&carr[i], &carr[j], switcher);
            // find minimum distance through all clusters, except the same cluster 
            // -1 is for first iteration
            if(current_dist < shortest_dist || shortest_dist == -1)
            {
                shortest_dist = current_dist;
                *c1 = i;
                *c2 = j;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar); 
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO

    FILE *file = fopen(filename, "r");

    if(!file)
    {
        fprintf(stderr, "File cannot be opened\n");
        return -1;
    }

    //i could use 21 (10+1+4+1+4+1)
    //length of max int + space(2x) + length of max x
    // + length of max y + newline
    char line[100];
    int count = 0, line_num = 0, obj_id, obj_x, obj_y;
    struct obj_t object;

    while(fgets(line, 100, file))
    {
        line_num++;

        if (line_num == 1)
        {
            // if first line is not a number, return error
            if (sscanf(line, "count=%d", &count) == 0)
            {
                fprintf(stderr, "Invalid file format (count)\n");
                fclose(file);
                return -1;
            }
            if (count <= 0)
            {
                fprintf(stderr, "Invalid file format (count)\n");
                fclose(file);
                return -1;
            }        

            *arr = malloc(count * sizeof(struct cluster_t));
            if (*arr == NULL)
            {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                return -1;
            }    
            continue;
        }

        // if there is more objects than specified in the first line
        // skip the rest of the file
        if (line_num - 1 > count)
            continue;

        // initialize cluster with capacity 1
        int malloc_test = init_cluster(&(*arr)[line_num-2], 1);
        if (malloc_test == -1)
        {
            fprintf(stderr, "Memory allocation error\n");
            fclose(file);
            return -1;
        }

        if (line_num > 1)
        {
            char end;

            // check if the line is valid
            if ((sscanf(line, "%d %d %d%[^\n]", 
                        &obj_id, &obj_x, &obj_y, &end)) != 3)
            {
                clear_clusters(*arr, line_num-1);
                // for(int i = 0; i <= line_num-2; i++)
                //     clear_cluster(&(*arr)[i]);
                // free(*arr);
                fclose(file);
                fprintf(stderr, "Invalid file format (object)\n");
                return -1;
            }
            if (obj_id < 0 || obj_x < 0 || obj_y < 0
                || obj_x > 1000 || obj_y > 1000)
            {
                clear_clusters(*arr, line_num-1);
                // for(int i = 0; i <= line_num-2; i++)
                //     clear_cluster(&(*arr)[i]);
                // free(*arr);
                fclose(file);
                fprintf(stderr, "Invalid file format (object) (%d)\n", obj_id);
                return -1;
            }
            object.id = obj_id;
            object.x = obj_x;
            object.y = obj_y;
            // add object to the empty cluster
            append_cluster(&(*arr)[line_num-2], object);
        }
    }

    // if there is less objects than count
    if (line_num - 1 < count)
    {
        clear_clusters(*arr, line_num-1);
        // for(int i = 0; i < line_num - 1; i++)
        //     clear_cluster(&(*arr)[i]);
        // free(*arr);
        fclose(file);
        fprintf(stderr, "Invalid file format (objects)\n");
        return -1;
    }

    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            // compare all ids
            if ((*arr)[i].obj[0].id == (*arr)[j].obj[0].id)
            {
                fprintf(stderr, "Invalid file format (object) %d and %d\n",
                        (*arr)[i].obj[0].id, (*arr)[j].obj[0].id);
                clear_clusters(*arr, count);        
                // for(int k = 0; k < count; k++)
                //     clear_cluster(&(*arr)[k]);
                // free(*arr);
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return count;

}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
    if (argc == 4 || argc == 3 || argc == 2)
    {
        int switcher = 0;
        if (argc == 4)
        {
            if (strcmp(argv[3], "-k") != 0)
            {
                fprintf(stderr, "Invalid argument\n");
                return 1;
            }
            else
            switcher = 1;
        }
        //or argc == 2
        if(argv[2] == NULL)
            argv[2] = "1";
        char *ptr = argv[2];
        //going through the string to check if it is a number
        //until the end of the string
        for (int i = 0; 1; i++)
        {
          if((ptr[i] < '0') || ('9' < ptr[i]))
          {
            fprintf(stderr, "Please enter an integer number\n");
            return 1;
          }
          //checking if the next symbol is \0
          if (ptr[i+1] == '\0')
             break;
        }
        int needed_clusters = atoi(argv[2]);
        if (needed_clusters == 0)
        {
            fprintf(stderr, "Invalid argument\n");
            return 1;
        }
        int count = load_clusters(argv[1], &clusters);
        if (count <= 0)
        {
            fprintf(stderr, "Invalid file format (count)\n");
            return 1;
        }
        if(needed_clusters > count)
        {
            clear_clusters(clusters, count);
            // for(int i = 0; i < count; i++)
            //     clear_cluster(&clusters[i]);
            // free(clusters);
            fprintf(stderr, "Invalid number of clusters\n");
            return 1;
        }

        int i, j;
        while(count > needed_clusters)
        {
            find_neighbours(clusters, count, &i, &j, switcher);
            merge_clusters(&clusters[i], &clusters[j]);
            count = remove_cluster(clusters, count, j);
        }

        print_clusters(clusters, count);

        clear_clusters(clusters, count);
        // for(int i = 0; i < count; i++)
        //     clear_cluster(&clusters[i]);
        // free(clusters);

    }
    else
    {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }

    return 0;
}