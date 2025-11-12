int d;
for (d = 0; d < 4; d++) {

S0
    int nx = cx + dx[d]; // coordenada x do vizinho
    
S1
    int ny = cy + dy[d]; // coordenada y do vizinho

S2
    // Ignora se o vizinho está fora dos limites do mapa
    if (nx < 0 || nx >= TAMANHO_MALHA || ny < 0 || ny >= TAMANHO_MALHA)
        continue;

    int nidx = IDX(nx, ny, TAMANHO_MALHA); // índice linear do vizinho
    uint8_t v = mapa->malha[nidx];         // valor da célula do mapa

    // Ignora células desconhecidas ou ocupadas (obstáculos)
    if (v == DESCONHECIDO || v == OCUPADO)
        continue;

    // Ignora se o nó já foi fechado
    if (closed_flag[nidx])
        continue;

    // Custo temporário = custo atual + 1 (movimento simples)
    uint32_t tent_g = gcur + 1;

    // Se o nó ainda não está aberto OU encontramos um caminho melhor
    if (!open_flag[nidx] || tent_g < g_cost[nidx]) {
        parent_dir[nidx] = (uint8_t)d; // registra a direção do pai
        g_cost[nidx] = tent_g;         // atualiza custo g
        f_cost[nidx] = tent_g + heuristica(nx, ny, dx_t, dy_t); // custo total estimado

        if (!open_flag[nidx]) {
            // Se ainda não estava aberto, insere no heap
            heap_insert(nidx);
            open_flag[nidx] = 1;
        } else {
            // Se já estava no heap, atualiza sua posição
            int hi = heap_index[nidx];
            if (hi >= 0) heapify_up(hi);
        }
    }
}

int d;
for (d = 0; d < 4; d++) {
    int nx = cx + dx[d];
    int ny = cy + dy[d];
    if (nx < 0 || nx >= TAMANHO_MALHA || ny < 0 || ny >= TAMANHO_MALHA) continue;

    int nidx = IDX(nx, ny, TAMANHO_MALHA);
    uint8_t v = mapa->malha[nidx];
    if (v == DESCONHECIDO || v == OCUPADO) continue;
    if (closed_flag[nidx]) continue;

    uint32_t tent_g = gcur + 1;
    if (!open_flag[nidx] || tent_g < g_cost[nidx]) {
        parent_dir[nidx] = (uint8_t)d;
        g_cost[nidx] = tent_g;
        f_cost[nidx] = tent_g + heuristica(nx, ny, dx_t, dy_t);
        if (!open_flag[nidx]) {
            heap_insert(nidx);
            open_flag[nidx] = 1;
        } else {
            int hi = heap_index[nidx];
            if (hi >= 0) heapify_up(hi);
        }
    }
}
}
/// Partes em HW

S* representam os estágios

Necessário:

Lógica de visitar cada vizinho, vai ser um pipeline
vamosu usar um fila para mandar as requisições e dados para o nios utilizar na heap

Incremento
Vetores e malha



void calcula_vizinhos_hw(const Mapa *mapa, int cur_idx, VizinhoHW *saida)
{

//Parte do FSM
    // Determina se o nó atual está na borda esquerda/direita
    int linha_ini = cur_idx;
    int linha_fim = linha_ini + TAMANHO_MALHA(largura) - 1;

    for (int d = 0; d < 4; d++) {
S0: cálculo do caminho a ser seguido e marcação de visitado

        saida->valid[d] = 0; //coloca vizinho como visitado
        int nidx = cur_idx + offsets[d];

S1: verificações em paralelo se é um vaminho válido, um and no final manda um sinal para a FSM indicando que acabou, passa sinal que não encontrou
        // Checa limites verticais (cima/baixo)
        if (d == 0 && nidx < 0) continue;
        if (d == 1 && nidx >= TAMANHO_MALHA_TOTAL) continue;


        // Checa bordas horizontais
        if (d == 2 && cur_idx == linha_ini) continue;  // esquerda
        if (d == 3 && cur_idx == linha_fim) continue;  // direita

S2: acesso na malha
        uint8_t celula = mapa->malha[nidx];
        
S3: verificação na célula e envio da requisição para a fila
        if (celula == OCUPADO || celula == DESCONHECIDO)
            continue;

        saida->valid[d] = 1;
        saida->nidx[d] = nidx;
    }
   
    uint32_t tent_g = gcur + 1;
    if (!open_flag[nidx] || tent_g < g_cost[nidx]) {
        parent_dir[nidx] = (uint8_t)d;
	g_cost[nidx] = tent_g;
        
S3.5: heurística dividida pelo custo, serão 2 somadores em cascata apenas, os outros vaõ ser paralelos
        f_cost_n = tent_g + heuristica(nx, ny, dx_t, dy_t);
        
S3.75: continuação da requisição
	f_cost[nidx] = f_cost_n
	
        if (!open_flag[nidx]) {
            heap_insert(nidx);
            open_flag[nidx] = 1;
        } else {
            int hi = heap_index[nidx];
            if (hi >= 0) heapify_up(hi);
        }
    }
}
