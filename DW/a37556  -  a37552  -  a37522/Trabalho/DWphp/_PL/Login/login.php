<body>
<br />
<div class="container" style="width:500px;">
    <h3 align="">PHP Login Script using PDO</h3><br />
    <form method="post">
        <label>email</label>
        <input type="text" name="email" class="form-control" />
        <br />
        <label>Password</label>
        <input type="password" name="password" class="form-control" />
        <br />
        <form method="POST">
            <input type="submit" name="login" class="btn btn-info" value="login" />
        </form>
        <form method="get"  action="?page=Login/Register" >
            <a href="?page=Login/Register" class="btn btn-info">Registar</a>
        </form>
        <form method="get"  action="?page=Login/Forgot" >
            <a href="?page=Login/Forgot" class="btn btn-info">resetPass</a>
        </form>
    </form>
</div>
<br />
</body>