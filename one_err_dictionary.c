#include <stdio.h>
#include <stdlib.h>

#define EPSILON 238
#define PRIME 101
#define BLOCKSIZE 3000

/* Declarations for hash table*/
int hash_table_length = 0;
long *currentblk = NULL;
int size_lft;
long *hash_table = NULL;
long *start = NULL;

long *create_hash_table() {

	long *tmp;

	if (currentblk == NULL || size_lft == 0) {

		if (currentblk == NULL) {
			currentblk = (long *) malloc(BLOCKSIZE * sizeof(long));
			start = currentblk;
			size_lft = BLOCKSIZE;
		} else {

			currentblk = ((long *) realloc(start,
					(hash_table_length + BLOCKSIZE) * sizeof(long)))
					+ hash_table_length;
			size_lft = BLOCKSIZE;
		}
	}
	tmp = currentblk++;
	size_lft -= 1;
	return tmp;

}

/*Declaration for hash table */

typedef char object_t;

typedef struct trie_n_t {
	struct trie_n_t *next[256];
/* possibly additional information*/} trie_node_t;

int create_trie_once = 0;
typedef trie_node_t node_t;

node_t *currentblock = NULL;
int size_left;
node_t *free_list = NULL;

node_t *get_node() {
	node_t *tmp;
	if (free_list != NULL) {
		tmp = free_list;
		free_list = free_list->next[0];
	} else {
		if (currentblock == NULL || size_left == 0) {
			currentblock = (node_t *) malloc( BLOCKSIZE * sizeof(node_t));
			size_left = BLOCKSIZE;
		}
		tmp = currentblock++;
		size_left -= 1;
	}
	return (tmp);
}

void return_node(node_t *node) {
	node->next[0] = free_list;
	free_list = node;
}

trie_node_t *create_trie(void) {
	trie_node_t *tmp_node;
	int i;
	tmp_node = get_node();
	for (i = 0; i < 256; i++)
		tmp_node->next[i] = NULL;
	return (tmp_node);
}

long power(int num, int pow) {
	int i = 1;
	long int sum = 1;
	while (i <= pow) {
		sum = sum * num;
		i++;
	}
	return sum;
}

long find(trie_node_t *trie, char *query_string) {

	trie_node_t *tmp_node, *tmp_node_rev;
	char *query_next, *query_next_rev;
	tmp_node = trie;
	tmp_node_rev = trie;
	int length = 0;
	int check = 0;

	length = strlen(query_string);
	//printf("length of key is %d\n", length);

	char *query_st_rev;
	query_st_rev = query_string;
	int rev_length = 0;
	rev_length = strlen(query_st_rev);
	while (*query_st_rev != '\0') {

		query_st_rev++;

	}

	char ins_key_rev[100];
	int key_index_rev = 0;
	char *ins_obj_rev;
	ins_obj_rev = (char *) malloc(100 * sizeof(char));

	query_st_rev--;
	while (rev_length > 0) {
		//printf("reverse character is %c\n", *query_st_rev);
		ins_key_rev[key_index_rev] = (int) *query_st_rev;
		key_index_rev++;
		query_st_rev--;
		rev_length--;
	}

	ins_key_rev[key_index_rev] = '\0';
	rev_length = strlen(query_string);

	int does_not_match[2];
	int does_not_match_rev[0];
	query_next = query_string;
	query_next_rev = ins_key_rev;
	int error_index = 0;
	int k = length - 1;
	int c = 0;
	long hash = 0;

	char *store;
	store = (char *) malloc(100 * sizeof(char));
	int str_len = 0;

	int at_index=0;
	int first = 0;
	while (*query_next != '\0') {

		if (tmp_node->next[(int) (*query_next)] == NULL) {

			length--;
			check++;
			does_not_match[0] = (int) (*query_next);
			printf("break at index %d, char is %c\n",at_index,does_not_match[0]);
			if (length == 0) {

				if (check > 1) {
					//printf("last character does not match\n");
					hash = 0;
					return hash; /* query string not found */
				} else {
					/* calculate EPSILON hash*/
					/*****
					 CALCULATE HASH VALUE
					 *****/
					hash = hash + (EPSILON * (power(PRIME, k)));
					k--;
					//printf("success in finding one error dict \n");
					return hash; /* one error found */

				}

			} else {
				if (check > 1) {
					hash = 0;
					return hash; /* query string not found */

				} else {

					/* check reverse of the string */
					int at_rev_index=0;
					while (*query_next_rev != '\0') {

						if (tmp_node_rev->next[(int) (*query_next_rev)] == NULL) {


							/*printf("char did not match is %c, 0 %c ,1 %c\n",
							 *query_next_rev, does_not_match[0],
							 does_not_match[1]);
*/
							int t = 0;
							int flag = 0;
							char *temp_store = store;
							while (t < str_len) {
								if (temp_store[t] == does_not_match[0]) {
									//printf("char does not match %c\n",temp_store[t]);
									flag++;
									break;
								}
								at_rev_index++;
								t++;
							}

							printf("break during rev at index %d\n",at_rev_index);
							if(at_rev_index == at_index){

								printf("CAUGGGt at char %c\n",(*query_next_rev));
							}
							if (does_not_match[0] == *query_next_rev
									|| does_not_match[1] == *query_next_rev
									|| flag == 1) {
								//printf("success in finding one error\n");
								/*****
								 CALCULATE HASH VALUE
								 *****/

								if (does_not_match[0] == *query_next_rev) {

									hash = hash + (EPSILON * (power(PRIME, c)));
								} else if (does_not_match[1] == *query_next_rev
										|| flag == 1) {
									//printf("flag is %d\n", flag);
									if (flag == 1) {
										first = 1;
									}
									if (first == 1) {
										int m = 0;
										hash = 0;
										int n = 0;
										while (m < str_len) {
											//printf("char are %c ,%d ,%d  %ld\n",store[m],store[m],n,hash);
											hash = hash + (store[m] * (power(
											PRIME, n)));
											n++;
											m++;
										}
										hash = hash
												+ (EPSILON * (power(PRIME, n)));
										//printf("hash is %ld\n",hash);

									} else {
										long hash_invalid;
										int key = k + 1;
										hash_invalid = (*query_next_rev
												* (power(PRIME, key)));
										hash = hash - hash_invalid;
										hash =
												hash
														+ (EPSILON
																* (power(PRIME,
																		key)));

									}

								}
								break;

							} else {
								hash = 0;
								return hash; /* query string not found, not a valid string */
							}
						} else {

							rev_length--;
							//printf("continue with reverse %c, at len %d\n", *query_next_rev,	rev_length);
							/*****
							 CALCULATE HASH VALUE
							 *****/
							hash = hash + (*query_next_rev * (power(PRIME, c)));
							store[str_len] = *query_next_rev;
							str_len++;
							c++;
							does_not_match_rev[0] = *query_next_rev;
							tmp_node_rev =
									tmp_node_rev->next[(int) (*query_next_rev)];

							query_next_rev += 1; /* move to next character of query */

						}
					}
					/* end of check reverse */
					break;

				}

			}
		} else {
			tmp_node = tmp_node->next[(int) (*query_next)];
			//printf("char %c\n", *query_next);
			does_not_match[1] = (int) (*query_next);
			first++;
			/*****
			 CALCULATE HASH VALUE
			 *****/
			hash = hash + (does_not_match[1] * (power(PRIME, k)));
			k--;

			length--;
			error_index++;
			at_index++;
			query_next += 1; /* move to next character of query */
		}
	}

	//printf("found object in find is %s\n",(object_t *) tmp_node->next[(int) '\0']);
	//printf("hash value is %ld\n", hash);
	return hash;
}

int insert(trie_node_t *trie, char *new_string, object_t *new_object) {
	trie_node_t *tmp_node, *new_node;
	char *query_next;
	int i;
	tmp_node = trie;
	query_next = new_string;
	while (*query_next != '\0') {
		if (tmp_node->next[(int) (*query_next)] == NULL) {
			new_node = get_node(); /* create missing node */
			for (i = 0; i < 256; i++)
				new_node->next[i] = NULL;
			tmp_node->next[(int) (*query_next)] = new_node;
		}
		/* move to next character */
		tmp_node = tmp_node->next[(int) (*query_next)];
		query_next += 1; /* move to next character */
	}
	if (tmp_node->next[(int) '\0'] != NULL)
		return (-1); /* string already exists, has object */
	else {
		//printf("object inserted %s\n", new_object);
		tmp_node->next[(int) '\0'] = (trie_node_t *) new_object;

	}
	return (0);
}

// process starts here

trie_node_t *tr;

trie_node_t *build(char *input) {

	int length = 0;
	char *temp_leng = input;
	char ins_key[100];
	int key_index = 0;
	//char obj[100];
	char *obj;
	obj = (char *) malloc(100 * sizeof(char));
	char *ins_obj;
	ins_obj = (char *) malloc(100 * sizeof(char));

	char *temp_rev = input;
	int rev_len = strlen(temp_rev);

	while (*temp_rev != '\0') {

		temp_rev++;

	}

	if (create_trie_once == 0) {

		tr = create_trie();
		create_trie_once++;
	}

	char ins_key_rev[100];
	int key_index_rev = 0;
	//char obj[100];
	char *obj_rev;
	obj_rev = (char *) malloc(100 * sizeof(char));
	char *ins_obj_rev;
	ins_obj_rev = (char *) malloc(100 * sizeof(char));

	temp_rev--;
	while (rev_len > 0) {
		//printf("reverse character is %c\n", *temp_rev);
		obj_rev[key_index_rev] = *temp_rev;
		ins_key_rev[key_index_rev] = (int) *temp_rev;
		key_index_rev++;
		temp_rev--;
		rev_len--;
	}

	obj_rev[key_index_rev] = '\0';
	ins_key_rev[key_index_rev] = '\0';
	ins_obj_rev = obj_rev;

	int success_rev;
	success_rev = insert(tr, ins_key_rev, ins_obj_rev);
	/*if (success_rev == 0)
	 printf("  insert successful, key = %s, object = %s\n", ins_key_rev,
	 ins_obj_rev);
	 else
	 printf("  insert failed, success = %d\n", success_rev);
	 */
	length = strlen(input);
	//printf("string length is %d\n", length);

	while (*temp_leng != '\0') {

		//printf("character is %c\n", *temp_leng);
		obj[key_index] = *temp_leng;
		ins_key[key_index] = (int) *temp_leng;
		key_index++;
		temp_leng++;
		length++;

	}
	obj[key_index] = '\0';
	ins_key[key_index] = '\0';
	ins_obj = obj;

	int success;
	success = insert(tr, ins_key, ins_obj);
	/*if (success == 0)
	 printf("  insert successful, key = %s, object = %s\n", ins_key,
	 ins_obj);
	 else
	 printf("  insert failed, success = %d\n", success);
	 */
	length = strlen(input);
	//printf("string length is %d\n", length);
	char *temp_inp = input;
	long hash = 0;
	int i = 0;
	int j = 0;
	i = length;
	int index = 0;
	while (i != 0) {

		index = length - i;
		hash = 0;
		j = 0;
		temp_inp = input;
		int k = 0;
		k = length - 1;
		while (*temp_inp != '\0') {

			if (j == index) {

				/*printf("EPSILON char %c and %d, at %d\n", *temp_inp, *temp_inp,
				 k);
				 */hash = hash + (EPSILON * (power(PRIME, k)));
				temp_inp++;
				j++;
				k--;
			} else {
				//printf("for char %c and %d, at %d\n", *temp_inp, *temp_inp, k);
				hash = hash + (*temp_inp * (power(PRIME, k)));
				temp_inp++;
				j++;
				k--;

			}

		}
		if (hash_table_length == 0) {

			hash_table = create_hash_table();
			hash_table_length++;
		} else {
			hash_table = create_hash_table();
			hash_table_length++;
		}

		*hash_table = hash;
		//printf("hash value is %ld\n", hash);
		i--;
	}

	hash = 0;
	temp_inp = input;
	int c = length - 1;

	while (*temp_inp != '\0') {

		hash = hash + (*temp_inp * (power(PRIME, c)));
		c--;
		temp_inp++;
	}
	if (hash_table_length == 0) {

		hash_table = create_hash_table();
		hash_table_length++;
	} else {
		hash_table = create_hash_table();
		hash_table_length++;
	}

	*hash_table = hash;
	//printf("------\n");
	return tr;
}

int query(char *input, trie_node_t *triee) {

	char *temp_leng = input;
	char *print_String = input;
	char ins_key[100];
	int key_index = 0;
	long hash = 0;

	//printf("check string %s\n", print_String);

	while (*temp_leng != '\0') {

		//printf("character is %c\n", *temp_leng);
		ins_key[key_index] = (int) *temp_leng;
		key_index++;
		temp_leng++;

	}
	ins_key[key_index] = '\0';

	hash = find(triee, ins_key);
	int i = 0;
	int success = 0;
	long *temp_start = start;
	if (hash == 0) {

		return 0;
	} else {
		for (i = 0; i < hash_table_length; ++i) {
			//printf("hash_table[%d] = %d\n", i, *temp_start++);

			if (hash == *temp_start) {
				success = 1;
			}
			temp_start++;
		}

		if (success == 1) {
			return 1;
		} else {
			return 0;
		}

	}

}

int main() {

	const char *strings[] = { "which", "there", "their", "about", "would",
			"these", "other", "words", "could", "write", "first", "water",
			"after", "where", "right", "think", "three", "years", "place",
			"sound", "great", "again", "still", "every", "small", "found",
			"those", "never", "under", "might", "while", "house", "world",
			"below", "asked", "going", "large", "until", "along", "shall",
			"being", "often", "earth", "began", "since", "study", "night",
			"light", "above", "paper", "parts", "young", "story", "point",
			"times", "heard", "whole", "white", "given", "means", "music",
			"miles", "thing", "today", "later", "using", "money", "lines",
			"order", "group", "among", "learn", "known", "space", "table",
			"early", "trees", "short", "hands", "state", "black", "shown",
			"stood", "front", "voice", "kinds", "makes", "comes", "close",
			"power", "lived", "vowel", "taken", "built", "heart", "ready",
			"quite", "class", "bring", "round", "horse", "shows", "piece",
			"green", "stand", "birds", "start", "river", "tried", "least",
			"field", "whose", "girls", "leave", "added", "color", "third",
			"hours", "moved", "plant", "doing", "names", "forms", "heavy",
			"ideas", "cried", "check", "floor", "begin", "woman", "alone",
			"plane", "spell", "watch", "carry", "wrote", "clear", "named",
			"books", "child", "glass", "human", "takes", "party", "build",
			"seems", "blood", "sides", "seven", "mouth", "solve", "north",
			"value", "death", "maybe" };
	trie_node_t *trie;
	printf("Build Trie with following strings :\n");
	int i = 0;
	for (i = 0; i < sizeof strings / sizeof *strings; i++) {
		printf("%d.", i + 1);
		printf("%s\n", strings[i]);
		trie = build(strings[i]);
	}

	printf("End building trie structure\n");
	printf("-------------------------\n");


	printf("Query for one error strings :\n");
	printf("\n");
	const char *query_strings[] = { "first", "foumd", "gound", "fuund", "foucd",
			"founm", "fuuud", "focmd", "foood", "words", "began", "cegan",
			"bngan", "beman", "begnn", "begak"

	};

	int success = 0;
	for (i = 0; i < sizeof query_strings / sizeof *query_strings; i++) {
		success = query(query_strings[i], trie);
		printf("%d. ", i + 1);

		printf("%s - ", query_strings[i]);
		if (success == 0) {
			printf("  string not found is %s \n", query_strings[i]);
		} else {
			printf("  find successful, string is %s\n", query_strings[i]);
		}

	}

	printf("END\n");
	return 0;
}
