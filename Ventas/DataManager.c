#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "DataManager.h"
#include "Cliente.h"
#include "Validaciones.h"
#include "Ventas.h"

int dm_saveAllCliente(ArrayList* nominaCliente)
{
    int i;
    int retorno=-1;

    FILE* pFile=fopen("Clientes.txt","w");
    void* pCliente=NULL;
    if(pFile!=NULL)
    {
        fprintf(pFile,"Id,Nombre,Apellido,Dni\n");
        for(i=0; i<al_len(nominaCliente); i++)
        {
            pCliente=al_get(nominaCliente,i);
            fprintf(pFile, "%d,%s,%s,%s\n",cliente_getId(pCliente),cliente_getNombre(pCliente),
                    cliente_getApellido(pCliente),cliente_getDni(pCliente));
            retorno=0;
        }
    }
    fclose(pFile);
    return retorno;
}

int dm_readAllCliente(ArrayList* nominaClientes)
{
    int retorno = -1;
    char bId[4096];
    char bNombre[4096];
    char bApellido[4096];
    char bDni[4096];

    FILE* pFile = fopen("Clientes.txt", "r");
    int maxId=0;

    Cliente* auxiliarCliente;

    if(pFile != NULL)
    {
        retorno = 0;
        if(fscanf(pFile,"%[^,],%[^,],%[^,],%[^\n]\n",bId,bNombre,bApellido,bDni)==4)
        {
            do
            {
                if(fscanf(pFile,"%[^,],%[^,],%[^,],%[^\n]\n",bId,bNombre,bApellido,bDni)==4)
                {
                    if( !val_validarUnsignedInt(bId) && !val_validarString(bNombre)&&
                            !val_validarString(bApellido) && !val_validarDni(bDni))
                    {
                        auxiliarCliente = cliente_new(bNombre,bApellido,bDni,atoi(bId),CLIENTE_ALTA);
                        al_add(nominaClientes,auxiliarCliente);
                        if(cliente_getId(auxiliarCliente)>maxId)
                        {
                            maxId=cliente_getId(auxiliarCliente);
                        }
                        retorno=maxId;
                    }
                }
            }
            while(!feof(pFile));
        }
    }

    return retorno;
}


int dm_saveAllVentas(ArrayList* nominaVentas)
{
    int i;
    int retorno=-1;

    FILE* pFile=fopen("Ventas.txt","w");
    void* pVenta=NULL;
    if(pFile!=NULL)
    {
        fprintf(pFile,"Id_venta,Id_cliente,codProducto,cantidad,precioUnitario\n");
        for(i=0; i<al_len(nominaVentas); i++)
        {
            pVenta=al_get(nominaVentas,i);
            fprintf(pFile, "%d,%d,%d,%d,%f\n", ventas_getId_ventas(pVenta),ventas_getId_clientes(pVenta),
                    ventas_getCodProducto(pVenta),ventas_getCantidad(pVenta),ventas_getPrecioUnitario(pVenta));
            retorno=0;
        }
    }
    fclose(pFile);
    return retorno;
}

int dm_readAllVentas(ArrayList* nominaVentas)
{
    int retorno = -1;
    char bId_venta[4096];
    char bId_cliente[4096];
    char bCod_producto[4096];
    char bCantidad[4096];
    char bPrecio[4096];

    FILE* pFile = fopen("Ventas.txt", "r");
    int maxId=0;

    Ventas* auxiliarVenta;

    if(pFile != NULL)
    {
        retorno = 0;
        if(fscanf(pFile,"%[^,],%[^,],%[^,],%[^,],%[^\n]\n",bId_venta,bId_cliente,
                  bCod_producto,bCantidad,bPrecio)==5)
        {
            do
            {
                if(fscanf(pFile,"%[^,],%[^,],%[^,],%[^,],%[^\n]\n",bId_venta,bId_cliente,
                          bCod_producto,bCantidad,bPrecio)==5)
                {
                    if( !val_validarUnsignedInt(bId_venta) && !val_validarUnsignedInt(bId_cliente) &&
                            !val_validarInt(bCod_producto) && !val_validarUnsignedInt(bCantidad) &&
                            !val_validarFloat(bPrecio))
                    {
                        auxiliarVenta = ventas_new(atoi(bId_venta),atoi(bId_cliente),atoi(bCod_producto),
                                                   atoi(bCantidad),atof(bPrecio),VENTA_ACTIVA);
                        al_add(nominaVentas,auxiliarVenta);
                        if(ventas_getId_ventas(auxiliarVenta) > maxId)
                        {
                            maxId = ventas_getId_ventas(auxiliarVenta);
                        }
                        retorno=maxId;
                    }
                }
            }
            while(!feof(pFile));
        }
    }

    return retorno;
}

int dm_generarInforme(ArrayList* nominaCliente, ArrayList* nominaVenta)
{
    int retorno=-1;
    Cliente* auxCliente;
    Ventas* auxVentas;
    int i=0, idCliente;
    float montoFacturado;
    FILE* pFile=fopen("Informes.txt","w");

    if(pFile!=NULL)
    {
        fprintf(pFile,"Id_venta,nombre_cliente,apellido_cliente,dni_cliente,codigo_producto,monto_facturado\n");
        for(i=0; i<al_len(nominaVenta); i++)
        {
            auxVentas = al_get(nominaVenta,i);
            idCliente = ventas_getId_clientes(auxVentas);
            auxCliente = cliente_findById(nominaCliente,idCliente);
            montoFacturado = ventas_getCantidad(auxVentas)*ventas_getPrecioUnitario(auxVentas);
            auxCliente=cliente_findById(nominaCliente,idCliente);
            if(auxCliente != NULL && auxVentas != NULL)
            {
                if(ventas_getEstado(auxVentas) == VENTA_ACTIVA && cliente_getEstado(auxCliente) == CLIENTE_ALTA)
                {
                    fprintf(pFile,"%d,%s,%s,%s,%d,%.2f\n", ventas_getId_ventas(auxVentas),cliente_getNombre(auxCliente),
                    cliente_getApellido(auxCliente),cliente_getDni(auxCliente),ventas_getCodProducto(auxVentas),montoFacturado);
                    printf("\nArchivo generado!\n");
                    retorno=0;
                }
            }

        }
    }
    fclose(pFile);
    return retorno;
}



