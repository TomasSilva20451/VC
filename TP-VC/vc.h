//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define VC_DEBUG

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Estrutura para armazenar dados da imagem
typedef struct
{
	unsigned char *data; // Ponteiro para os dados da imagem
	int width, height;	 // Largura e altura da imagem
	int channels;		 // Número de canais da imagem (ex: 1 para binário, 3 para RGB)
	int levels;			 // Níveis de cor (1 para binário, 255 para cinza)
	int bytesperline;	 // Número de bytes por linha (width * channels)
} IVC;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROT�TIPOS DE FUN��ES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUN��ES: ALOCAR E LIBERTAR UMA IMAGEM
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);

// FUN��ES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);

// Declaração da função de etiquetagem de blobs
int vc_binary_blob_labelling(IVC *src, IVC *dst);

// NEGATIVO DE IMAGENS
int vc_gray_negative(IVC *srcdst);
int vc_rgb_negative(IVC *srcdst);

// EXTRAIR CORES
int vc_rgb_get_red_gray(IVC *srcdst);
int vc_rgb_get_blue_gray(IVC *srcdst);
int vc_rgb_get_green_gray(IVC *srcdst);
