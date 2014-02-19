namespace StandAloneLauncher
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.serverToggle = new System.Windows.Forms.Button();
            this.lanBroadcast = new System.Windows.Forms.CheckBox();
            this.serverName = new System.Windows.Forms.TextBox();
            this.label_serverName = new System.Windows.Forms.Label();
            this.listenPort = new System.Windows.Forms.NumericUpDown();
            this.label_listenPort = new System.Windows.Forms.Label();
            this.panel_serverOptions = new System.Windows.Forms.Panel();
            this.panel_commands = new System.Windows.Forms.Panel();
            this.timeLimit = new System.Windows.Forms.NumericUpDown();
            this.mapName = new System.Windows.Forms.ComboBox();
            this.gameModes = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.forceStart = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.labelClientsConnected = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.nrOfClients = new System.Windows.Forms.NumericUpDown();
            this.buttonStartGame = new System.Windows.Forms.Button();
            this.panel_clientArea = new System.Windows.Forms.Panel();
            this.ServerInfoTextArea = new System.Windows.Forms.RichTextBox();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.clientInfoBox = new System.Windows.Forms.ListBox();
            this.panel_CommanArea = new System.Windows.Forms.Panel();
            this.panelServerCommands = new System.Windows.Forms.Panel();
            this.dataProtocolFields = new System.Windows.Forms.TableLayoutPanel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.button2 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.buttonsAtBottom = new System.Windows.Forms.TableLayoutPanel();
            this.buttonAddNewDataField = new System.Windows.Forms.Button();
            this.buttonExecuteSend = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).BeginInit();
            this.panel_serverOptions.SuspendLayout();
            this.panel_commands.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.timeLimit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nrOfClients)).BeginInit();
            this.panel_clientArea.SuspendLayout();
            this.panel_CommanArea.SuspendLayout();
            this.panelServerCommands.SuspendLayout();
            this.dataProtocolFields.SuspendLayout();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.buttonsAtBottom.SuspendLayout();
            this.SuspendLayout();
            // 
            // serverToggle
            // 
            this.serverToggle.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.serverToggle.Location = new System.Drawing.Point(0, 83);
            this.serverToggle.Name = "serverToggle";
            this.serverToggle.Size = new System.Drawing.Size(241, 20);
            this.serverToggle.TabIndex = 0;
            this.serverToggle.Text = "Start server";
            this.serverToggle.UseVisualStyleBackColor = true;
            this.serverToggle.Click += new System.EventHandler(this.button1_serverToggle_Click);
            // 
            // lanBroadcast
            // 
            this.lanBroadcast.AutoSize = true;
            this.lanBroadcast.Location = new System.Drawing.Point(9, 58);
            this.lanBroadcast.Name = "lanBroadcast";
            this.lanBroadcast.Size = new System.Drawing.Size(95, 17);
            this.lanBroadcast.TabIndex = 1;
            this.lanBroadcast.Text = "Lan Broadcast";
            this.lanBroadcast.UseVisualStyleBackColor = true;
            // 
            // serverName
            // 
            this.serverName.Location = new System.Drawing.Point(81, 3);
            this.serverName.Name = "serverName";
            this.serverName.Size = new System.Drawing.Size(95, 20);
            this.serverName.TabIndex = 3;
            this.serverName.Text = "GameServer";
            // 
            // label_serverName
            // 
            this.label_serverName.AutoSize = true;
            this.label_serverName.Location = new System.Drawing.Point(6, 6);
            this.label_serverName.Name = "label_serverName";
            this.label_serverName.Size = new System.Drawing.Size(69, 13);
            this.label_serverName.TabIndex = 4;
            this.label_serverName.Text = "Server Name";
            // 
            // listenPort
            // 
            this.listenPort.Location = new System.Drawing.Point(81, 29);
            this.listenPort.Maximum = new decimal(new int[] {
            65000,
            0,
            0,
            0});
            this.listenPort.Minimum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.listenPort.Name = "listenPort";
            this.listenPort.Size = new System.Drawing.Size(95, 20);
            this.listenPort.TabIndex = 5;
            this.listenPort.Value = new decimal(new int[] {
            15151,
            0,
            0,
            0});
            // 
            // label_listenPort
            // 
            this.label_listenPort.AutoSize = true;
            this.label_listenPort.Location = new System.Drawing.Point(6, 31);
            this.label_listenPort.Name = "label_listenPort";
            this.label_listenPort.Size = new System.Drawing.Size(56, 13);
            this.label_listenPort.TabIndex = 4;
            this.label_listenPort.Text = "Listen port";
            // 
            // panel_serverOptions
            // 
            this.panel_serverOptions.Controls.Add(this.serverName);
            this.panel_serverOptions.Controls.Add(this.listenPort);
            this.panel_serverOptions.Controls.Add(this.serverToggle);
            this.panel_serverOptions.Controls.Add(this.label_listenPort);
            this.panel_serverOptions.Controls.Add(this.lanBroadcast);
            this.panel_serverOptions.Controls.Add(this.label_serverName);
            this.panel_serverOptions.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel_serverOptions.Location = new System.Drawing.Point(0, 0);
            this.panel_serverOptions.Name = "panel_serverOptions";
            this.panel_serverOptions.Size = new System.Drawing.Size(241, 103);
            this.panel_serverOptions.TabIndex = 6;
            // 
            // panel_commands
            // 
            this.panel_commands.Controls.Add(this.timeLimit);
            this.panel_commands.Controls.Add(this.mapName);
            this.panel_commands.Controls.Add(this.gameModes);
            this.panel_commands.Controls.Add(this.label3);
            this.panel_commands.Controls.Add(this.forceStart);
            this.panel_commands.Controls.Add(this.label2);
            this.panel_commands.Controls.Add(this.label4);
            this.panel_commands.Controls.Add(this.label5);
            this.panel_commands.Controls.Add(this.labelClientsConnected);
            this.panel_commands.Controls.Add(this.label1);
            this.panel_commands.Controls.Add(this.nrOfClients);
            this.panel_commands.Controls.Add(this.buttonStartGame);
            this.panel_commands.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel_commands.Location = new System.Drawing.Point(0, 103);
            this.panel_commands.Name = "panel_commands";
            this.panel_commands.Size = new System.Drawing.Size(241, 188);
            this.panel_commands.TabIndex = 7;
            this.panel_commands.Visible = false;
            // 
            // timeLimit
            // 
            this.timeLimit.Location = new System.Drawing.Point(112, 89);
            this.timeLimit.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.timeLimit.Name = "timeLimit";
            this.timeLimit.Size = new System.Drawing.Size(123, 20);
            this.timeLimit.TabIndex = 11;
            this.timeLimit.ThousandsSeparator = true;
            this.timeLimit.Value = new decimal(new int[] {
            15,
            0,
            0,
            0});
            // 
            // mapName
            // 
            this.mapName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mapName.FormattingEnabled = true;
            this.mapName.IntegralHeight = false;
            this.mapName.Items.AddRange(new object[] {
            "Set directory"});
            this.mapName.Location = new System.Drawing.Point(72, 7);
            this.mapName.Name = "mapName";
            this.mapName.Size = new System.Drawing.Size(163, 21);
            this.mapName.TabIndex = 10;
            this.mapName.SelectedIndexChanged += new System.EventHandler(this.mapName_SelectedIndexChanged);
            // 
            // gameModes
            // 
            this.gameModes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gameModes.FormattingEnabled = true;
            this.gameModes.Items.AddRange(new object[] {
            "Free-for-all",
            "Team death-match"});
            this.gameModes.Location = new System.Drawing.Point(77, 61);
            this.gameModes.Name = "gameModes";
            this.gameModes.Size = new System.Drawing.Size(158, 21);
            this.gameModes.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(95, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Time limit (minutes)";
            // 
            // forceStart
            // 
            this.forceStart.AutoSize = true;
            this.forceStart.Checked = true;
            this.forceStart.CheckState = System.Windows.Forms.CheckState.Checked;
            this.forceStart.Location = new System.Drawing.Point(12, 120);
            this.forceStart.Name = "forceStart";
            this.forceStart.Size = new System.Drawing.Size(115, 17);
            this.forceStart.TabIndex = 1;
            this.forceStart.Text = "Ignore empty lobby";
            this.forceStart.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 69);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Game mode";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 15);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(57, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Map name";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(23, 147);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(81, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Lobby clients: 0";
            // 
            // labelClientsConnected
            // 
            this.labelClientsConnected.AutoSize = true;
            this.labelClientsConnected.Location = new System.Drawing.Point(131, 147);
            this.labelClientsConnected.Name = "labelClientsConnected";
            this.labelClientsConnected.Size = new System.Drawing.Size(80, 13);
            this.labelClientsConnected.TabIndex = 8;
            this.labelClientsConnected.Text = "Game clients: 0";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 38);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Client limit";
            // 
            // nrOfClients
            // 
            this.nrOfClients.Location = new System.Drawing.Point(72, 34);
            this.nrOfClients.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.nrOfClients.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.nrOfClients.Name = "nrOfClients";
            this.nrOfClients.Size = new System.Drawing.Size(163, 20);
            this.nrOfClients.TabIndex = 7;
            this.nrOfClients.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // buttonStartGame
            // 
            this.buttonStartGame.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonStartGame.Location = new System.Drawing.Point(0, 166);
            this.buttonStartGame.Name = "buttonStartGame";
            this.buttonStartGame.Size = new System.Drawing.Size(241, 22);
            this.buttonStartGame.TabIndex = 6;
            this.buttonStartGame.Text = "Start game";
            this.buttonStartGame.UseVisualStyleBackColor = true;
            this.buttonStartGame.Click += new System.EventHandler(this.buttonStartGame_Click);
            // 
            // panel_clientArea
            // 
            this.panel_clientArea.Controls.Add(this.ServerInfoTextArea);
            this.panel_clientArea.Controls.Add(this.splitter1);
            this.panel_clientArea.Controls.Add(this.clientInfoBox);
            this.panel_clientArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel_clientArea.Location = new System.Drawing.Point(241, 0);
            this.panel_clientArea.Name = "panel_clientArea";
            this.panel_clientArea.Size = new System.Drawing.Size(494, 616);
            this.panel_clientArea.TabIndex = 8;
            // 
            // ServerInfoTextArea
            // 
            this.ServerInfoTextArea.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ServerInfoTextArea.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ServerInfoTextArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ServerInfoTextArea.Font = new System.Drawing.Font("GulimChe", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ServerInfoTextArea.ForeColor = System.Drawing.SystemColors.Info;
            this.ServerInfoTextArea.Location = new System.Drawing.Point(0, 269);
            this.ServerInfoTextArea.Name = "ServerInfoTextArea";
            this.ServerInfoTextArea.ReadOnly = true;
            this.ServerInfoTextArea.Size = new System.Drawing.Size(494, 347);
            this.ServerInfoTextArea.TabIndex = 1;
            this.ServerInfoTextArea.Text = "";
            // 
            // splitter1
            // 
            this.splitter1.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitter1.Location = new System.Drawing.Point(0, 264);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(494, 5);
            this.splitter1.TabIndex = 2;
            this.splitter1.TabStop = false;
            // 
            // clientInfoBox
            // 
            this.clientInfoBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.clientInfoBox.FormattingEnabled = true;
            this.clientInfoBox.Location = new System.Drawing.Point(0, 0);
            this.clientInfoBox.Name = "clientInfoBox";
            this.clientInfoBox.Size = new System.Drawing.Size(494, 264);
            this.clientInfoBox.TabIndex = 0;
            // 
            // panel_CommanArea
            // 
            this.panel_CommanArea.Controls.Add(this.panelServerCommands);
            this.panel_CommanArea.Controls.Add(this.panel_commands);
            this.panel_CommanArea.Controls.Add(this.panel_serverOptions);
            this.panel_CommanArea.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel_CommanArea.Location = new System.Drawing.Point(0, 0);
            this.panel_CommanArea.Name = "panel_CommanArea";
            this.panel_CommanArea.Size = new System.Drawing.Size(241, 616);
            this.panel_CommanArea.TabIndex = 9;
            // 
            // panelServerCommands
            // 
            this.panelServerCommands.Controls.Add(this.dataProtocolFields);
            this.panelServerCommands.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelServerCommands.Location = new System.Drawing.Point(0, 291);
            this.panelServerCommands.Name = "panelServerCommands";
            this.panelServerCommands.Size = new System.Drawing.Size(241, 85);
            this.panelServerCommands.TabIndex = 8;
            this.panelServerCommands.Visible = false;
            // 
            // dataProtocolFields
            // 
            this.dataProtocolFields.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.dataProtocolFields.ColumnCount = 1;
            this.dataProtocolFields.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 21F));
            this.dataProtocolFields.Controls.Add(this.panel2, 0, 1);
            this.dataProtocolFields.Controls.Add(this.tableLayoutPanel1, 0, 0);
            this.dataProtocolFields.Controls.Add(this.buttonsAtBottom, 0, 2);
            this.dataProtocolFields.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataProtocolFields.Location = new System.Drawing.Point(0, 0);
            this.dataProtocolFields.Margin = new System.Windows.Forms.Padding(0);
            this.dataProtocolFields.Name = "dataProtocolFields";
            this.dataProtocolFields.RowCount = 3;
            this.dataProtocolFields.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 27F));
            this.dataProtocolFields.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 27F));
            this.dataProtocolFields.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.dataProtocolFields.Size = new System.Drawing.Size(241, 85);
            this.dataProtocolFields.TabIndex = 9;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.button2);
            this.panel2.Controls.Add(this.textBox1);
            this.panel2.Controls.Add(this.comboBox1);
            this.panel2.Controls.Add(this.numericUpDown1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(4, 32);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(233, 21);
            this.panel2.TabIndex = 0;
            // 
            // button2
            // 
            this.button2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button2.FlatAppearance.BorderColor = System.Drawing.Color.Black;
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button2.Location = new System.Drawing.Point(184, 0);
            this.button2.Margin = new System.Windows.Forms.Padding(0);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(49, 21);
            this.button2.TabIndex = 0;
            this.button2.Text = "remove";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBox1.Location = new System.Drawing.Point(120, 0);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(64, 20);
            this.textBox1.TabIndex = 12;
            // 
            // comboBox1
            // 
            this.comboBox1.Dock = System.Windows.Forms.DockStyle.Left;
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.IntegralHeight = false;
            this.comboBox1.Items.AddRange(new object[] {
            "netBool;",
            "netChar;",
            "netUChar;",
            "netShort;",
            "netUShort;",
            "netInt;",
            "netUInt;",
            "netInt64;",
            "netUInt64;",
            "netFloat;",
            "netDouble;",
            "netCharPtr;"});
            this.comboBox1.Location = new System.Drawing.Point(42, 0);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(78, 21);
            this.comboBox1.TabIndex = 10;
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Dock = System.Windows.Forms.DockStyle.Left;
            this.numericUpDown1.Location = new System.Drawing.Point(0, 0);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(42, 20);
            this.numericUpDown1.TabIndex = 11;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 35.29412F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 64.70588F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 112F));
            this.tableLayoutPanel1.Controls.Add(this.label6, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label7, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.label8, 2, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(4, 4);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 27.05882F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(233, 21);
            this.tableLayoutPanel1.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(4, 1);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(18, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "ID";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(46, 1);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(31, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "Type";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(122, 1);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(34, 13);
            this.label8.TabIndex = 8;
            this.label8.Text = "Value";
            // 
            // buttonsAtBottom
            // 
            this.buttonsAtBottom.ColumnCount = 2;
            this.buttonsAtBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.buttonsAtBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.buttonsAtBottom.Controls.Add(this.buttonAddNewDataField, 0, 0);
            this.buttonsAtBottom.Controls.Add(this.buttonExecuteSend, 1, 0);
            this.buttonsAtBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonsAtBottom.Location = new System.Drawing.Point(4, 60);
            this.buttonsAtBottom.Name = "buttonsAtBottom";
            this.buttonsAtBottom.RowCount = 1;
            this.buttonsAtBottom.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.buttonsAtBottom.Size = new System.Drawing.Size(233, 21);
            this.buttonsAtBottom.TabIndex = 10;
            // 
            // buttonAddNewDataField
            // 
            this.buttonAddNewDataField.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonAddNewDataField.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonAddNewDataField.Location = new System.Drawing.Point(0, 0);
            this.buttonAddNewDataField.Margin = new System.Windows.Forms.Padding(0);
            this.buttonAddNewDataField.Name = "buttonAddNewDataField";
            this.buttonAddNewDataField.Size = new System.Drawing.Size(116, 21);
            this.buttonAddNewDataField.TabIndex = 1;
            this.buttonAddNewDataField.Text = "Add field";
            this.buttonAddNewDataField.UseVisualStyleBackColor = true;
            this.buttonAddNewDataField.Click += new System.EventHandler(this.buttonAddNewDataField_Click);
            // 
            // buttonExecuteSend
            // 
            this.buttonExecuteSend.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonExecuteSend.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonExecuteSend.Location = new System.Drawing.Point(116, 0);
            this.buttonExecuteSend.Margin = new System.Windows.Forms.Padding(0);
            this.buttonExecuteSend.Name = "buttonExecuteSend";
            this.buttonExecuteSend.Size = new System.Drawing.Size(117, 21);
            this.buttonExecuteSend.TabIndex = 0;
            this.buttonExecuteSend.Text = "Send";
            this.buttonExecuteSend.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(735, 616);
            this.Controls.Add(this.panel_clientArea);
            this.Controls.Add(this.panel_CommanArea);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormClosingEvent);
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).EndInit();
            this.panel_serverOptions.ResumeLayout(false);
            this.panel_serverOptions.PerformLayout();
            this.panel_commands.ResumeLayout(false);
            this.panel_commands.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.timeLimit)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nrOfClients)).EndInit();
            this.panel_clientArea.ResumeLayout(false);
            this.panel_CommanArea.ResumeLayout(false);
            this.panelServerCommands.ResumeLayout(false);
            this.dataProtocolFields.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.buttonsAtBottom.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button serverToggle;
        private System.Windows.Forms.CheckBox lanBroadcast;
        private System.Windows.Forms.TextBox serverName;
        private System.Windows.Forms.Label label_serverName;
        private System.Windows.Forms.NumericUpDown listenPort;
        private System.Windows.Forms.Label label_listenPort;
        private System.Windows.Forms.Panel panel_serverOptions;
        private System.Windows.Forms.Panel panel_commands;
        private System.Windows.Forms.Panel panel_clientArea;
        private System.Windows.Forms.ListBox clientInfoBox;
        private System.Windows.Forms.RichTextBox ServerInfoTextArea;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.Button buttonStartGame;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nrOfClients;
        private System.Windows.Forms.ComboBox gameModes;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown timeLimit;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox forceStart;
        private System.Windows.Forms.Label labelClientsConnected;
        private System.Windows.Forms.Panel panel_CommanArea;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox mapName;
        private System.Windows.Forms.Panel panelServerCommands;
        private System.Windows.Forms.Button buttonExecuteSend;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.TableLayoutPanel dataProtocolFields;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button buttonAddNewDataField;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TableLayoutPanel buttonsAtBottom;
    }
}

