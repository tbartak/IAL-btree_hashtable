/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  if (tree != NULL)
  {
    (*tree) = NULL;
  }
  
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while (tree != NULL) // prohledavame strom dokud ma nejake prvky nebo najdeme hledany prvek
  {
    if (key == tree->key) // pokud je momentalni uzel shodny s klicem, tak jsme nasli hledany prvek
    {
      (*value) = tree->value;
      return true;
    }
    else
    {
      if (key < tree->key) // jestlize je klic mensi nez momentalni uzel, tak se presuneme do leveho podstromu
      {
        tree = tree->left;
      }
      else // jestlize je vetsi, tak se presuneme do praveho podstromu
      {
        tree = tree->right;
      }
    }
  }
  return false; // jestlize uzel nebyl nalezen, vratime false
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (tree != NULL)
  {
    bst_node_t* currentRoot = (*tree); // aktualni uzel
    if (currentRoot == NULL) // pokud je strom prazdny, tak muzeme rovnou uzel pridat
    {
      currentRoot = malloc(sizeof(bst_node_t));
      if (currentRoot == NULL) // chyba pri alokaci
      {
        return;
      }
      //ulozime do currentRoot vsechna data
      currentRoot->key = key;
      currentRoot->value = value;
      currentRoot->left = NULL;
      currentRoot->right = NULL;
      (*tree) = currentRoot;
      // free(currentRoot);
      return;
    }


    while (currentRoot != NULL) // prohledavame postupne cely strom, abychom nas uzel vlozili na spravne misto
    {
      if (key == currentRoot->key) // pokud uzel se zadanym klicem existuje, nahradime jeho hodnotu
      {
        currentRoot->value = value;
        // free(currentRoot);
        return;
      }
      else
      {
        if (key < currentRoot->key) // pokud je klic mensi nez momentalni uzel, budeme prochazet levy podstrom
        {
          if (currentRoot->left == NULL) // pokud je uzel nalevo prazdny, tak muzeme momentalni uzel jiz vlozit
          {
            bst_node_t* newRoot;
            newRoot = malloc(sizeof(bst_node_t));
            newRoot->key = key;
            newRoot->value = value;
            newRoot->left = NULL;
            newRoot->right = NULL;
            currentRoot->left = newRoot; // propojime momentalni uzel s novym
            // free(newRoot);
          }
          else // jestli jeste uzel nalevo prazdny nebyl, tak se posuneme na levy podstrom
          {
            currentRoot = currentRoot->left; 
          }
        }
        else // jinak budeme prochazet jeho pravy podstrom
        {
          if (currentRoot->right == NULL)
          {
            bst_node_t* newRoot;
            newRoot = malloc(sizeof(bst_node_t));
            newRoot->key = key;
            newRoot->value = value;
            newRoot->left = NULL;
            newRoot->right = NULL;
            currentRoot->right = newRoot; // propojime momentalni uzel s novym
            // free(newRoot);
          }
          else // jestli jeste uzel napravo prazdny nebyl, tak se posuneme na pravy podstrom
          {
            currentRoot = currentRoot->right;            
          }
        }
      }
    }
    (*tree) = currentRoot;
    free(currentRoot);
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree) != NULL)
  {
    while ((*tree)->right != NULL) // hledame nejpravejsi uzel, cyklus skonci az najde nejpravejsi uzel
    {
      tree = &((*tree)->right);
    }
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    
    bst_node_t* tmp = (*tree);
    (*tree) = (*tree)->left;
    free(tmp);
    tmp = NULL;
  }
  
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if ((*tree) != NULL) // pokud neni strom prazdny
  {
    bst_node_t* currentRoot = (*tree);
    bst_node_t* previousRoot = NULL;
    while (currentRoot != NULL) // hledame prvek, ktery je potreba odstranit
    {
      if (key == currentRoot->key) // nasli jsme prvek, ktery chceme odstranit
      {
        break;
      }
      else
      {
        if (key < currentRoot->key)
        {
          previousRoot = currentRoot;
          currentRoot = currentRoot->left;
        }
        else
        {
          if (key > currentRoot->key)
          {
            previousRoot = currentRoot;
            currentRoot = currentRoot->right;
          } 
        } 
      }
    }
    if (currentRoot == NULL) // prvek s danym klicem neexistuje
    {
      return;
    }
    
    if (currentRoot->left == NULL && currentRoot->right == NULL) // prvek nema potomky
    {
      if (previousRoot != NULL && previousRoot->left == currentRoot) // odstranime ukazatel z otce na leveho syna
      {
        // free(currentRoot);
        previousRoot->left = NULL;
      }
      else
      {
        if (previousRoot != NULL && previousRoot->right == currentRoot) // odstranime ukazatel z otce na praveho syna
        {
          previousRoot->right = NULL;
        }
        else // uzel nema ani potomky ani rodice, je jako jediny prvek ve strome
        {
          (*tree) = NULL;
        }
      }
    }
    else
    {
      if (currentRoot->left != NULL && currentRoot->right == NULL) // uzel ma pouze levy podstrom
      {
        if (previousRoot->left == currentRoot) // pokud je odstranovany uzel levy syn, tak propojime otce s levym podstromem syna
        {
          previousRoot->left = currentRoot->left;
        }
        else
        {
          if (previousRoot->right == currentRoot) // pokud je odstranovany uzel pravy syn, tak propojime otce s levym podstromem syna
          {
            previousRoot->right = currentRoot->left;
          }
          else // nema otce
          {
            (*tree) = currentRoot->left;
          }
        } 
      }
      else
      {
        if (currentRoot->right != NULL && currentRoot->left == NULL) // uzel ma pouze levy podstrom
        {
          if (previousRoot->left == currentRoot) // pokud je odstranovany uzel levy syn, tak propojime otce s levym podstromem syna
          {
            previousRoot->left = currentRoot->right;
          }
          else
          {
            if (previousRoot->right == currentRoot) // pokud je odstranovany uzel pravy syn, tak propojime otce s levym podstromem syna
            {
              previousRoot->right = currentRoot->right;
            }
            else // nema otce
            {
              *tree = currentRoot->right;
            }
          } 
        }
        else // ma oba dva podstromy
        {
          bst_replace_by_rightmost(currentRoot, &(currentRoot->left));
          return;
        } 
      }
    }
    free(currentRoot);    
    currentRoot = NULL;
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if ((*tree) != NULL) // jen pokud neni prazdny strom
  {
    stack_bst_t* stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) // chyba pri alokaci
    {
      return;
    }
    stack_bst_init(stack); 
    
    while ((*tree) != NULL || stack_bst_empty(stack) == false) // prochazime strom do doby, nez je prazdny a uvolneny z pameti
    {
      if ((*tree) == NULL && stack_bst_empty(stack) == false) // pokud je uzel prazdny
      {
        (*tree) = stack_bst_pop(stack); // ulozime si vrchni prvek ze zasobniku
      }
      else
      {
        if ((*tree)->right != NULL) // pravy podstrom si ulozime na zasobnik pro pozdejsi zpracovani, pokud existuje
        {
          stack_bst_push(stack, (*tree)->right);
        }
        bst_node_t* currentRoot = (*tree);
        (*tree) = (*tree)->left;
        free(currentRoot);
        currentRoot = NULL;
      }
    }
    (*tree) = NULL;
    free(stack);
  }
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  // ROOT, LEFT, RIGHT
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree); 
    bst_add_node_to_items(tree, items);
    tree = tree->left;
  }
  
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL)
  {
    stack_bst_t* stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) // chyba alokace
    {
      return;
    }
    stack_bst_init(stack);
    bst_leftmost_preorder(tree, stack, items);
    while (stack_bst_empty(stack) == false) // dokud se nevyprazdni zasobnik
    {
      tree = stack_bst_pop(stack);
      bst_leftmost_preorder(tree->right, stack, items);
    }
    free(stack);
  }
  
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  // LEFT, ROOT, RIGHT
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree); 
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL)
  {
    stack_bst_t* stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) // chyba alokace
    {
      return;
    }
    stack_bst_init(stack);
    bst_leftmost_inorder(tree, stack);
    while (stack_bst_empty(stack) == false) // dokud se nevyprazdni zasobnik
    {
      tree = stack_bst_pop(stack);
      bst_add_node_to_items(tree, items);
      bst_leftmost_inorder(tree->right, stack);
    }
    free(stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  // LEFT, RIGHT, ROOT
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree); 
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL)
  {
    stack_bst_t* stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) // chyba alokace
    {
      return;
    }
    stack_bst_init(stack);    
    stack_bool_t* visit_stack = malloc(sizeof(stack_bool_t));
    if (stack == NULL) // chyba alokace
    {
      return;
    }    
    stack_bool_init(visit_stack);

    bst_leftmost_postorder(tree, stack, visit_stack);
    while (stack_bst_empty(stack) == false) // dokud se nevyprazdni zasobnik
    {
      tree = stack_bst_pop(stack);
      bool first_visit = stack_bool_pop(visit_stack);
      if (first_visit == true) // uzel byl navstiven poprve, ulozime ho do zasobniku a na visit_stack se pushne false, protoze jiz byl navstiven
      {
        stack_bst_push(stack, tree);
        stack_bool_push(visit_stack, false);
        bst_leftmost_postorder(tree->right, stack, visit_stack);
      }
      else
      {
        bst_add_node_to_items(tree, items);        
      }
    }
    free(stack);
    free(visit_stack);
  }
}