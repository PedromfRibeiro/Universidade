<?php

require_once dirname(__FILE__) . '/../_DAL/UtilizadorDAL.php';


class Utilizador
{
    public $idUtilizador;
    public $Nome;
    public $pass;
    public $Data_Registo;
    public $Autorizacao;
    public $Data_Nascimento;
    public $email;
    public $code_hash;
    public $Verify;

    public function __construct($idUtilizador=null, $Nome=null, $pass=null, $Data_Registo=null, $Autorizacao=null, $Data_Nascimento=null, $email=null, $code_hash=null, $Verify=null)
    {
        $this->idUtilizador = $idUtilizador;
        $this->Nome = $Nome;
        $this->pass = $pass;
        $this->Data_Registo = $Data_Registo;
        $this->Autorizacao = $Autorizacao;
        $this->Data_Nascimento = $Data_Nascimento;
        $this->email = $email;
        $this->code_hash = $code_hash;
        $this->Verify = $Verify;
    }


    public function Create()
    {
        return UtilizadorDAL::CreateDAL($this);
    }

    public function Read()
    {
        return UtilizadorDAL::ReadDAL($this);


    }

    public function ReadALL()
    {
        return UtilizadorDAL::ReadALLDAL();


    }

    public function Update()
    {
        return UtilizadorDAL::UpdateDAL($this);
    }

    public function Delete()
    {
        return UtilizadorDAL::DeleteDAL($this);
    }

    public function CreateTable()
    {
        return UtilizadorDAL::CreateTable();


    }

    public function ReadVerify()
    {
        return UtilizadorDAL::ReadVerifyDAL($this);

    }

    public function ReadEmail()
    {
        return UtilizadorDAL::ReadEmailDAL($this);


    }
    public function ReadEmailOBJ()
    {
        return UtilizadorDAL::ReadEmailOBJDAL($this);


    }

    public function ReadEmailHash()
    {
        return UtilizadorDAL::ReadEmailHashDAL($this);


    }
    public function ReadbyID()
    {
        return UtilizadorDAL::ReadbyIDDAL($this);


    }

    public function UpdateVerify()
    {
        return UtilizadorDAL::UpdateVerifyDAL($this);
    }

    public function ReadALLLimit($a,$b)
    {
        return UtilizadorDAL::ReadALLDALLIMIT($a,$b);

    }

    public function Counterrows()
    {
        return UtilizadorDAL::Counterrows();

    }
}