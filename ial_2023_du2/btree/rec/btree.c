/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree != NULL) // prohledavame strom pouze pokud ma nejake prvky
  {
    if (tree->key == key) // pokud je momentalni uzel shodny s klicem, tak jsme nasli hledany prvek
    {
      (*value) = tree->value;
      return true;
    }
    else
    {
      if (key < tree->key) // jestlize je klic mensi nez momentalni uzel, tak prohledame levy podstrom
      {
        return bst_search(tree->left, key, value);
      }
      else // jestlize je vetsi, prohledame pravy podstrom
      {
        return bst_search(tree->right, key, value);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
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
      // ulozime do currentRoot vsechna data
      currentRoot->key = key;
      currentRoot->value = value;
      currentRoot->left = NULL;
      currentRoot->right = NULL;

      (*tree) = currentRoot;
    }
    if (key == currentRoot->key) // pokud uzel se zadanym klicem existuje, nahradime jeho hodnotu
    {
      currentRoot->value = value;
      return; //
    }
    else
    {
      if (key < currentRoot->key) // pokud je klic mensi nez momentalni uzel, budeme vkladat do jeho leveho podstromu
      {
        bst_insert(&(currentRoot->left), key, value);
      }
      else // jinak budeme vkladat do jeho praveho podstromu
      {
        bst_insert(&(currentRoot->right), key, value);
      } 
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (*tree != NULL/* && target != NULL*/)
  {
    if ((*tree)->right == NULL) // nasli jsme nejpravejsi uzel, nahradime klic a hodnotu uzlu target timto uzlem
    {
      target->key = (*tree)->key;
      target->value = (*tree)->value;
      // tento uzel budeme chtit odstranit a uvolnit jeho pamet
      bst_node_t* tmp = (*tree); // ulozime si odstranovany uzel do pomocne promenne
      (*tree) = (*tree)->left; // a nasledne posuneme ukazatel do jeho leveho podstromu
      free(tmp);
      tmp = NULL;
    }
    else // pokud jsme zatim nenasli nejpravejsi uzel, tak hledame rekurzivne dal
    {
      bst_replace_by_rightmost(target, &(*tree)->right);
    }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if ((*tree) != NULL)
  {
    if (key == (*tree)->key) // nasli jsme uzel, ktery chceme odstranit
    {
      if ((*tree)->left == NULL && (*tree)->right == NULL) // kontrola jake ma potomky, pripadne jestli vubec nejake ma
      {
        free((*tree));
        (*tree) = NULL;
      }
      else
      {
        if ((*tree)->left == NULL && (*tree)->right != NULL) // ma jen pravy podstrom jako potomky
        {
          bst_node_t* tmp = (*tree);
          (*tree) = (*tree)->right; // propojime zbytek stromu s jeho pravym podstromem
          free(tmp);
        }
        else
        {
          if ((*tree)->left != NULL && (*tree)->right == NULL) // ma jen levy podstrom jako potomky
          {
            bst_node_t* tmp = (*tree);
            (*tree) = (*tree)->left; // propojime zbytek stromu s jeho levym podstromem
            free(tmp);
          }
          else // uzel ma i levy i pravy podstrom jako potomky
          {
            bst_replace_by_rightmost((*tree), &((*tree)->left));
            return;
          }
        }
      } 
    }
    else
    {
      if (key < (*tree)->key) // prvek, ktery chceme smazat je v levem podstromu, protoze jeho klic je nizsi, nez klic momentalniho uzlu
      {
        bst_delete(&((*tree)->left), key);
      }
      else // prvek, ktery chceme smazat je v pravem podstromu, protoze jeho klic je vyssi, nez klic momentalniho uzlu
      {
        bst_delete(&((*tree)->right), key);
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if ((*tree) != NULL)
  {
    if ((*tree)->left != NULL) // pokud levy podstrom neni prazdny, tak se ho rekurzivne zbavujeme
    {
      bst_dispose(&((*tree)->left));
    }
    if ((*tree)->right != NULL) // pokud pravy podstrom neni prazdny, tak se ho rekurzivne zbavujeme
    {
      bst_dispose(&((*tree)->right));
    }
    
    // uvolneni uzlu z pameti
    free((*tree));
    (*tree) = NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
// PreOrder: Root, Left, RIght (R, L, RI)
  if (tree != NULL)
  {
    bst_add_node_to_items(tree, items); // prvne se u PreOrder zpracovava koren
    if (tree->left != NULL) // pak levy podstrom, ktery se stane novym korenem
    {
      bst_preorder(tree->left, items);
    }
    if (tree->right != NULL) // nakonec se zpracuje jeste pravy podstrom, ktery se stane novym korenem
    {
      bst_preorder(tree->right, items);
    }
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
// InOrder: Left, Root, RIght (L, R, RI)
  if (tree != NULL)
  {
    if (tree->left != NULL) // prvne se zpracovava levy podstrom, ktery se stane novym korenem
    {
      bst_inorder(tree->left, items);
    }
    bst_add_node_to_items(tree, items); // pak se u InOrder zpracovava koren
    if (tree->right != NULL) // nakonec se zpracuje jeste pravy podstrom, ktery se stane novym korenem
    {
      bst_inorder(tree->right, items);
    }
  }
  
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
// PostOrder: Left, RIght, Root (L, RI, R)
  if (tree != NULL)
  {
    if (tree->left != NULL) // prvne se zpracovava levy podstrom, ktery se stane novym korenem
    {
      bst_postorder(tree->left, items);
    }
    if (tree->right != NULL) // pak se zpracuje jeste pravy podstrom, ktery se stane novym korenem
    {
      bst_postorder(tree->right, items);
    }
    bst_add_node_to_items(tree, items); // nakonec se u PostOrder zpracovava koren
  }
}
