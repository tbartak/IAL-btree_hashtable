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
        // prevod znaku na male pismeno a pokud to bude jiny symbol nez ' ' nebo pismeno, tak nahradi znakem '_'
        currentInput = myToLower(currentInput);
        currentInput = isSymbol(currentInput);
        int value = 0;
        bst_search((*tree), currentInput, &value); // najdeme si momentalni hodnotu s pocetnosti vyskytu daneho znaku
        bst_insert(tree, currentInput, (value + 1)); // vkladame znak, pokud se tam vyskytoval, tak zvysime jeho hodnotu, aby odpovidala poctu znaku v retezci
        input++;
    }

}

// pomocna funkce na rekurzivni vybalancovani stromu
bst_node_t* createBalancedTree(bst_items_t* balancedItemsArray, int start, int end)
{
    if (start > end) // pokud se rekurzivne stane, ze zacatek bude vetsi nez konec, tak to znamena, ze je tahle cast stromu vybalancovana
    {
        return NULL;
    }
    int mid = (start + end) / 2; // najdeme si stred pole
    bst_node_t* treeRoot = balancedItemsArray->nodes[mid]; // uzel se kterym chceme pracovat se nachazi v poli na indexu mid
    treeRoot->left = createBalancedTree(balancedItemsArray, start, mid - 1); // rekurzivni volani pro levy podstrom
    treeRoot->right = createBalancedTree(balancedItemsArray, mid + 1, end); // rekurzivni volani pro pravy podstrom
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
    bst_node_t *new;
    // inicializace balanced_items, do kterych se bude ukladat serazeny strom
    bst_items_t* balanced_items = malloc(sizeof(bst_items_t));
    balanced_items->capacity = 0;
    balanced_items->nodes = NULL;
    balanced_items->size = 0;
    bst_inorder(*tree, balanced_items); // serazeni prvku pomoci funkce inOrder, prvky jsou ulozene v poli uzlu v balanced_items->nodes
    new = createBalancedTree(balanced_items, 0, balanced_items->size - 1); // vytvoreni noveho vyvazeneho stromu
    *tree = new;
    if(balanced_items != NULL) { // vratime balanced_items do puvodniho stavu
        if (balanced_items->capacity > 0)
        {
            free(balanced_items->nodes);
        }
        balanced_items->capacity = 0;
        balanced_items->size = 0;
    }
    free(balanced_items); // uvolneni naalokovane pameti pro balanced_items
}