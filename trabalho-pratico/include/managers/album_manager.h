#ifndef ALBUM_MANAGER_H
#define ALBUM_MANAGER_H

#include <glib.h>

#include "albums.h"
#include "artist_manager.h"

typedef struct album_manager *Album_Manager;

/**
 * @brief Cria um gestor de álbuns.
 * 
 * Aloca memória para um gestor de álbuns, assim como inicializa a sua hash table.
 * Destaca-se que, juntamente com a sua criação, estabele-se que a função @b free_album
 * será a responsável por liberar a memória de cada entrada presente na tabela, quando
 * está for eliminada.
 * 
 * @return Gestor de álbuns.
 */
Album_Manager create_album_manager ();

/**
 * @brief Libera a memória ocupada pelo gestor de álbuns.
 * 
 * Dá free do struct e da hash table deste, dado um gestor @p am.  
 * 
 * @param am Gestor cujo struct e tabela serão destruídos.
 */
void free_album_manager (Album_Manager am);


/**
 * @brief Verifica se um determinado álbum está presente na hash table do gestor.
 * 
 * Se relaciona com a função @b search_album_by_id, que dado um @p id e um gestor @p am, devolve o
 * álbum cujo @p id se especifica, caso este esteja presente no gestor, ou @b NULL, caso contrário.
 * Sabendo disso, o que a função @b album_exists faz resume-se a invocar a @b search_album_by_id,
 * com os parâmetros que esta necessita, e verifica se o retorno da chamada é nulo (neste caso
 * simbolizando que o @p id dado não é chave de nenhuma entrada da tabela @p am).
 * 
 * @param id Identificador único do álbum a ser procurado na tabela.
 * @param am Gestor cuja tabela será revistada a procura do @p id.
 * @return Devolve @p TRUE se a tabela contiver o @p id; @b FALSE, caso contrário.
 */
gboolean album_exists (int id, Album_Manager am);


/**
 * @brief Armazena todos os álbuns e preenche um parâmetro de cada artista.
 *
 * Verifica se caminho dado é válido e em seguida inicializa as componentes que armazenarão temporariamente
 * os campos de cada linha do ficheiro. Atribui-se a um GArray o(s) artista(s) do álbum daquela linha e soma-se
 * uma unidade ao campo @b n_albums de cada artista autor dessa obra (este GArray é posteriormente eliminado devido
 * a sua utilidade restringir-se somente a essa tarefa). Além disso, o álbum presente na linha em questão é adicionado
 * na hash table do @p album_man.
 * 
 * @param album_path Caminho para o ficheiro CSV dos álbuns.
 * @param album_man Album Manager que armazenará os álbuns.
 * @param art_man Contém a hash table dos artistas que terão seu número de álbuns incrementado.
 * @param interativo Indica se o o programa está no modo interativo ou não.
 */
int store_Album (char *album_path, Album_Manager album_man, Art_Manager art_man, int interativo);

#endif