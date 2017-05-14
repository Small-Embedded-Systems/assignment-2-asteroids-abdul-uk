/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to use them */
struct ship {
		int heading;
		float speed;//in pixels per second
		float drag;
    coordinate_t p;
    vector_t     v;
};

/* initial struts for building linked lists */
struct rock {
    int heading;
    coordinate_t p;
	  vector_t     v;
	  float ttl;
    struct rock *next;
};

struct missile {
	int heading;
	float ttl;
  coordinate_t p;
	vector_t v;
  struct missile *next;
};

void physics(void);

void shoot_missile();
void initialise_missiles();
missile *allocate_missile_node(void);
void initialise_missiles();

void initialise_asteroids();
rock *allocate_rock_node(void);
void free_rock_node(rock *n);
