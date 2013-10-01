#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
        struct node *Right;
        struct node *Left;
        int Value;
} Node;

int InsertNode(Node **leaf, int value)
{
	int success = 0;

	if ( (*leaf) == 0 )
	{
		*leaf = (Node*) malloc(sizeof(Node));
		(*leaf)->Value = value;
		//(*leaf)->Right = 0;
		//(*leaf)->Left = 0;
		success = 1;
	}
	else if (value > (*leaf)->Value)
		success = InsertNode(&(*leaf)->Right, value);
	else if (value < (*leaf)->Value)
		success = InsertNode(&(*leaf)->Left, value);
	return success;
}

void PrintInOrder(Node **root)
{
	if ((*root))
	{
		PrintInOrder(&(*root)->Left);
		printf("%d\n", (*root)->Value);
		PrintInOrder(&(*root)->Right);
	}
}

int main(void)
{
	Node *ptr = 0;

	int true = 1;
	while (true)
	{
		unsigned int input = 0;
		printf("Ingresa 1 para insertar un Nodo, 2 para imprimir InOrden.\nIngresa 3 para salir:\n");
		scanf("%d", &input);

		switch (input)
		{
			case 1:
                        {
                                printf("\nIngresa el valor del Nodo:\n");
                                int value = 0;
                                scanf("%d", &value);
                                if (&ptr)
				{
	                                if (InsertNode(&ptr, value))
        	                                printf("Se agrego un Nodo \n");
	                                else
        	                                printf("Ocurrio un error al tratar de agregar un Nodo\n");
				}
				else //ROOT
				{
					Node root;
					root.Value = value;
					root.Right = 0;
					root.Left = 0;
					printf("Se agrego un Nodo \n");
				}
                                break;
                        }
                        case 2:
                        {
                                printf("Recorrido INORDEN:\n");
                                PrintInOrder(&ptr);
                                break;
			}
			case 3:
			{
				true = 0;
				break;
			}
			default:
			{
				printf("Valor no valido\n");
				break;
			}
		}
	}

        return 0;
}
