/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

// /*
//  * Vyhledání uzlu v stromu.
//  *
//  * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
//  * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
//  * value zůstává nezměněná.
//  * 
//  */
// bool exa_bst_search(bst_node_t *tree, char key, int *value) {
//   if (tree != NULL) // prohledavame strom pouze pokud ma nejake prvky
//   {
//     if (tree->key == key) // pokud je momentalni uzel shodny s klicem, tak jsme nasli hledany prvek
//     {
//       (*value) = tree->value;
//       return true;
//     }
//     else
//     {
//       if (key < tree->key) // jestlize je klic mensi nez momentalni uzel, tak prohledame levy podstrom
//       {
//         return exa_bst_search(tree->left, key, value);
//       }
//       else // jestlize je vetsi, prohledame pravy podstrom
//       {
//         return exa_bst_search(tree->right, key, value);
//       }
//     }
//   }
//   return false; // jestlize uzel nebyl nalezen, vratime false
// }

// /*
//  * Vložení uzlu do stromu.
//  *
//  * Pokud uzel se zadaným klíčem už ve stromu existuje, zvyšte jeho hodnotu.
//  * Jinak vložte nový listový uzel.
//  *
//  * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
//  * uzlu obsahuje jenom menší klíče, pravý větší. 
//  *
//  */
// void exa_bst_insert(bst_node_t **tree, char key) {
//   if (tree != NULL)
//   {
//     bst_node_t* currentRoot = (*tree); // aktualni uzel
//     if (currentRoot == NULL) // pokud je strom prazdny, tak muzeme rovnou uzel pridat
//     {
//       currentRoot = malloc(sizeof(bst_node_t));
//       if (currentRoot == NULL) // chyba pri alokaci
//       {
//         return;
//       }
//       // ulozime do currentRoot vsechna data
//       currentRoot->key = key;
//       currentRoot->value = 1;
//       currentRoot->left = NULL;
//       currentRoot->right = NULL;

//       (*tree) = currentRoot;
//     }
//     if (key == currentRoot->key) // pokud uzel se zadanym klicem existuje, zvysime jeho hodnotu
//     {
//         int value = 0;
//         bst_search(*tree, key, &value);
//         value++;
//       currentRoot->value = value;
//       return; //
//     }
//     else
//     {
//       if (key < currentRoot->key) // pokud je klic mensi nez momentalni uzel, budeme vkladat do jeho leveho podstromu
//       {
//         exa_bst_insert(&(currentRoot->left), key);
//       }
//       else // jinak budeme vkladat do jeho praveho podstromu
//       {
//         exa_bst_insert(&(currentRoot->right), key);
//       } 
//     }
//   }
// }

int myToLower(char c) // predela velke znaky na male
{
    return (c >= 'A' && c <='Z') ? (c + 32) : (c);    
}

int isSymbol(char c) // predela znaky krome a-z a mezery na '_', funkce pocita s tim, ze retezec byl drive preveden na mala pismena
{
    if ((c < 'a' || c > 'z') && c != ' ')
    {
        c = '_';
        return c;
    }
    return c;
}

void HeightBT (bst_node_t *tree, int *max) // funkce na urceni vysky stromu
{
    int heightLeft, heightRight;
    if (tree != NULL)
    {
        HeightBT(tree->left, &heightLeft);
        HeightBT(tree->right, &heightRight);
        if (heightLeft > heightRight) 
        {
            *max = heightLeft + 1;
        } 
        else 
        {
            *max = heightRight + 1;
        }
    } // if ptr != NULL
    else 
    {
        *max = 0;
    }
}

bst_node_t* CopyR (bst_node_t* tree) // rekurzivni kopirovani stromu
{
    bst_node_t* newTree;
    if (tree != NULL){
        newTree = malloc(sizeof(bst_node_t));
        // if (newTree == NULL) // chyba alokace
        // {
        //     return;
        // }
        newTree->key = tree->key;
        newTree->value = tree->value;
        newTree->left = CopyR(tree->left);
        newTree->right = CopyR(tree->right);
        return newTree;
    } 
    else 
    {
        return NULL;
    }
}

bool TestWBT (bst_node_t *tree, int *count) // test, zda-li je strom vahove vyvazeny
{
    bool left_balanced, right_balanced;
    int left_count, right_count;
    if (tree != NULL)
    {
        left_balanced = TestWBT(tree->left, &left_count);
        right_balanced = TestWBT(tree->right, &right_count);
        *count = left_count + right_count + 1;
        return (left_balanced && right_balanced && (abs(left_count - right_count) <= 1));
    }
    else
    {
        *count = 0;
        return true;
    }
}

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);
    while (*input != '\0')
    {
        char currentInput = *input;
        currentInput = myToLower(currentInput);
        currentInput = isSymbol(currentInput);
        int value = 0;
        bst_search((*tree), currentInput, &value);
        bst_insert(tree, currentInput, (value + 1));
        input++;
    }

}

// bst_items_t* exa_bst_init_items() {
//   bst_items_t* items = malloc(sizeof(bst_items_t));
//   items->capacity = 0;
//   items->nodes = NULL;
//   items->size = 0;
//   return items;
// }

void exa_bst_print_items(bst_items_t *items) {
  printf("Traversed items:\n");
    for (int i = 0; i < items->size; i++)
    {
        bst_print_node(items->nodes[i]);
    }
  printf("\n");
}

// void exa_bst_reset_items (bst_items_t *items) {
//   if(items != NULL) {
//     if (items->capacity > 0)
//     {
//       free(items->nodes);
//     }
//     items->capacity = 0;
//     items->size = 0;
//   }
// }

bst_node_t* createBalancedTree(bst_items_t* balancedItemsArray, int start, int end)
{
    if (start > end)
    {
        return NULL;
    }
    int mid = (start + end)/2;
    bst_node_t* treeRoot = balancedItemsArray->nodes[mid];
    printf("Momentalne zpracovavany uzel: %c\n", treeRoot->key);
    // printf("")
    treeRoot->left = createBalancedTree(balancedItemsArray, start, mid - 1);
    treeRoot->right = createBalancedTree(balancedItemsArray, mid + 1, end);
    return treeRoot;
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // nakonec musime stary strom disposenout

    bst_node_t *new;
    bst_items_t* balanced_items = malloc(sizeof(bst_items_t));
    balanced_items->capacity = 0;
    balanced_items->nodes = NULL;
    balanced_items->size = 0;
    bst_inorder(*tree, balanced_items);
    new = createBalancedTree(balanced_items, 0, balanced_items->size - 1);
    exa_bst_print_items(balanced_items); // ted je to ocividne spravne, inorder funguje
    *tree = new;
    if(balanced_items != NULL) {
        if (balanced_items->capacity > 0)
        {
            free(balanced_items->nodes);
        }
        balanced_items->capacity = 0;
        balanced_items->size = 0;
    }
    free(balanced_items);
    // bst_dispose(&(*tree));
}