namespace StandAloneLauncher
{
    partial class NoEdgeWindow
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
            this.serverName = new System.Windows.Forms.TextBox();
            this.label_serverName = new System.Windows.Forms.Label();
            this.listenPort = new System.Windows.Forms.NumericUpDown();
            this.label_listenPort = new System.Windows.Forms.Label();
            this.panel_serverOptions = new System.Windows.Forms.Panel();
            this.timeLimit = new System.Windows.Forms.NumericUpDown();
            this.mapName = new System.Windows.Forms.ComboBox();
            this.gameModes = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.forceStart = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.nrOfClients = new System.Windows.Forms.NumericUpDown();
            this.buttonStart = new System.Windows.Forms.Button();
            this.panel_clientArea = new System.Windows.Forms.Panel();
            this.ServerInfoTextArea = new System.Windows.Forms.RichTextBox();
            this.tabControl_options = new System.Windows.Forms.TabControl();
            this.tabPage_consol = new System.Windows.Forms.TabPage();
            this.textBox_consonl_textfield = new System.Windows.Forms.TextBox();
            this.panel_consol_interactive = new System.Windows.Forms.Panel();
            this.button_consol_excexute = new System.Windows.Forms.Button();
            this.textBox_name = new System.Windows.Forms.TextBox();
            this.textBox_time = new System.Windows.Forms.TextBox();
            this.textBox_clients = new System.Windows.Forms.TextBox();
            this.textBox_ip = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.tabPage_info = new System.Windows.Forms.TabPage();
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).BeginInit();
            this.panel_serverOptions.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.timeLimit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nrOfClients)).BeginInit();
            this.panel_clientArea.SuspendLayout();
            this.tabControl_options.SuspendLayout();
            this.tabPage_consol.SuspendLayout();
            this.panel_consol_interactive.SuspendLayout();
            this.tabPage_info.SuspendLayout();
            this.SuspendLayout();
            // 
            // serverName
            // 
            this.serverName.BackColor = System.Drawing.Color.Gray;
            this.serverName.Location = new System.Drawing.Point(112, 3);
            this.serverName.Name = "serverName";
            this.serverName.Size = new System.Drawing.Size(123, 20);
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
            this.listenPort.BackColor = System.Drawing.Color.Gray;
            this.listenPort.Location = new System.Drawing.Point(112, 29);
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
            this.listenPort.Size = new System.Drawing.Size(123, 20);
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
            this.panel_serverOptions.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.panel_serverOptions.Controls.Add(this.timeLimit);
            this.panel_serverOptions.Controls.Add(this.serverName);
            this.panel_serverOptions.Controls.Add(this.mapName);
            this.panel_serverOptions.Controls.Add(this.gameModes);
            this.panel_serverOptions.Controls.Add(this.listenPort);
            this.panel_serverOptions.Controls.Add(this.label3);
            this.panel_serverOptions.Controls.Add(this.label_listenPort);
            this.panel_serverOptions.Controls.Add(this.forceStart);
            this.panel_serverOptions.Controls.Add(this.label_serverName);
            this.panel_serverOptions.Controls.Add(this.label2);
            this.panel_serverOptions.Controls.Add(this.buttonStart);
            this.panel_serverOptions.Controls.Add(this.label4);
            this.panel_serverOptions.Controls.Add(this.nrOfClients);
            this.panel_serverOptions.Controls.Add(this.label1);
            this.panel_serverOptions.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel_serverOptions.ForeColor = System.Drawing.Color.White;
            this.panel_serverOptions.Location = new System.Drawing.Point(0, 0);
            this.panel_serverOptions.MinimumSize = new System.Drawing.Size(241, 242);
            this.panel_serverOptions.Name = "panel_serverOptions";
            this.panel_serverOptions.Size = new System.Drawing.Size(241, 242);
            this.panel_serverOptions.TabIndex = 6;
            // 
            // timeLimit
            // 
            this.timeLimit.BackColor = System.Drawing.Color.Gray;
            this.timeLimit.Location = new System.Drawing.Point(112, 137);
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
            this.mapName.Location = new System.Drawing.Point(112, 55);
            this.mapName.Name = "mapName";
            this.mapName.Size = new System.Drawing.Size(123, 21);
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
            this.gameModes.Location = new System.Drawing.Point(112, 109);
            this.gameModes.Name = "gameModes";
            this.gameModes.Size = new System.Drawing.Size(123, 21);
            this.gameModes.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 144);
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
            this.forceStart.Location = new System.Drawing.Point(12, 168);
            this.forceStart.Name = "forceStart";
            this.forceStart.Size = new System.Drawing.Size(55, 17);
            this.forceStart.TabIndex = 1;
            this.forceStart.Text = "Lobby";
            this.forceStart.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 117);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Game mode";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 63);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(57, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Map name";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 86);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Client limit";
            // 
            // nrOfClients
            // 
            this.nrOfClients.BackColor = System.Drawing.Color.Gray;
            this.nrOfClients.Location = new System.Drawing.Point(112, 82);
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
            this.nrOfClients.Size = new System.Drawing.Size(123, 20);
            this.nrOfClients.TabIndex = 7;
            this.nrOfClients.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // buttonStart
            // 
            this.buttonStart.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonStart.Location = new System.Drawing.Point(64, 191);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(97, 27);
            this.buttonStart.TabIndex = 6;
            this.buttonStart.Text = "Start Server";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStartGame_Click);
            // 
            // panel_clientArea
            // 
            this.panel_clientArea.Controls.Add(this.tabControl_options);
            this.panel_clientArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel_clientArea.Location = new System.Drawing.Point(241, 0);
            this.panel_clientArea.MinimumSize = new System.Drawing.Size(293, 146);
            this.panel_clientArea.Name = "panel_clientArea";
            this.panel_clientArea.Size = new System.Drawing.Size(293, 230);
            this.panel_clientArea.TabIndex = 8;
            // 
            // ServerInfoTextArea
            // 
            this.ServerInfoTextArea.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.ServerInfoTextArea.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ServerInfoTextArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ServerInfoTextArea.Font = new System.Drawing.Font("GulimChe", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ServerInfoTextArea.ForeColor = System.Drawing.SystemColors.Info;
            this.ServerInfoTextArea.Location = new System.Drawing.Point(0, 0);
            this.ServerInfoTextArea.Name = "ServerInfoTextArea";
            this.ServerInfoTextArea.ReadOnly = true;
            this.ServerInfoTextArea.Size = new System.Drawing.Size(285, 178);
            this.ServerInfoTextArea.TabIndex = 1;
            this.ServerInfoTextArea.Text = "";
            // 
            // tabControl_options
            // 
            this.tabControl_options.Controls.Add(this.tabPage_info);
            this.tabControl_options.Controls.Add(this.tabPage_consol);
            this.tabControl_options.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl_options.Location = new System.Drawing.Point(0, 0);
            this.tabControl_options.Name = "tabControl_options";
            this.tabControl_options.SelectedIndex = 0;
            this.tabControl_options.Size = new System.Drawing.Size(293, 230);
            this.tabControl_options.TabIndex = 7;
            // 
            // tabPage_consol
            // 
            this.tabPage_consol.BackColor = System.Drawing.Color.DimGray;
            this.tabPage_consol.Controls.Add(this.ServerInfoTextArea);
            this.tabPage_consol.Controls.Add(this.panel_consol_interactive);
            this.tabPage_consol.Location = new System.Drawing.Point(4, 22);
            this.tabPage_consol.Margin = new System.Windows.Forms.Padding(0);
            this.tabPage_consol.Name = "tabPage_consol";
            this.tabPage_consol.Size = new System.Drawing.Size(285, 204);
            this.tabPage_consol.TabIndex = 0;
            this.tabPage_consol.Text = "Consol";
            // 
            // textBox_consonl_textfield
            // 
            this.textBox_consonl_textfield.BackColor = System.Drawing.Color.Black;
            this.textBox_consonl_textfield.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox_consonl_textfield.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBox_consonl_textfield.ForeColor = System.Drawing.Color.White;
            this.textBox_consonl_textfield.Location = new System.Drawing.Point(0, 0);
            this.textBox_consonl_textfield.MaxLength = 512;
            this.textBox_consonl_textfield.MinimumSize = new System.Drawing.Size(223, 26);
            this.textBox_consonl_textfield.Multiline = true;
            this.textBox_consonl_textfield.Name = "textBox_consonl_textfield";
            this.textBox_consonl_textfield.Size = new System.Drawing.Size(223, 26);
            this.textBox_consonl_textfield.TabIndex = 2;
            this.textBox_consonl_textfield.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_consonl_textfield_KeyPress);
            // 
            // panel_consol_interactive
            // 
            this.panel_consol_interactive.Controls.Add(this.button_consol_excexute);
            this.panel_consol_interactive.Controls.Add(this.textBox_consonl_textfield);
            this.panel_consol_interactive.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel_consol_interactive.Location = new System.Drawing.Point(0, 178);
            this.panel_consol_interactive.Name = "panel_consol_interactive";
            this.panel_consol_interactive.Size = new System.Drawing.Size(285, 26);
            this.panel_consol_interactive.TabIndex = 3;
            // 
            // button_consol_excexute
            // 
            this.button_consol_excexute.BackColor = System.Drawing.Color.Gray;
            this.button_consol_excexute.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button_consol_excexute.FlatAppearance.BorderColor = System.Drawing.Color.Black;
            this.button_consol_excexute.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_consol_excexute.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.button_consol_excexute.Location = new System.Drawing.Point(223, 0);
            this.button_consol_excexute.Margin = new System.Windows.Forms.Padding(0);
            this.button_consol_excexute.MaximumSize = new System.Drawing.Size(62, 26);
            this.button_consol_excexute.MinimumSize = new System.Drawing.Size(62, 26);
            this.button_consol_excexute.Name = "button_consol_excexute";
            this.button_consol_excexute.Size = new System.Drawing.Size(62, 26);
            this.button_consol_excexute.TabIndex = 4;
            this.button_consol_excexute.Text = "Excecute";
            this.button_consol_excexute.UseVisualStyleBackColor = false;
            this.button_consol_excexute.Click += new System.EventHandler(this.button_consol_excexute_Click);
            // 
            // textBox_name
            // 
            this.textBox_name.BackColor = System.Drawing.Color.Gray;
            this.textBox_name.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox_name.Location = new System.Drawing.Point(116, 7);
            this.textBox_name.Margin = new System.Windows.Forms.Padding(0);
            this.textBox_name.Name = "textBox_name";
            this.textBox_name.ReadOnly = true;
            this.textBox_name.Size = new System.Drawing.Size(164, 21);
            this.textBox_name.TabIndex = 0;
            this.textBox_name.Text = "Name";
            // 
            // textBox_time
            // 
            this.textBox_time.BackColor = System.Drawing.Color.Gray;
            this.textBox_time.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox_time.Location = new System.Drawing.Point(116, 34);
            this.textBox_time.Margin = new System.Windows.Forms.Padding(0);
            this.textBox_time.Name = "textBox_time";
            this.textBox_time.ReadOnly = true;
            this.textBox_time.Size = new System.Drawing.Size(164, 21);
            this.textBox_time.TabIndex = 0;
            this.textBox_time.Text = "Time";
            // 
            // textBox_clients
            // 
            this.textBox_clients.BackColor = System.Drawing.Color.Gray;
            this.textBox_clients.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox_clients.Location = new System.Drawing.Point(116, 60);
            this.textBox_clients.Margin = new System.Windows.Forms.Padding(0);
            this.textBox_clients.Name = "textBox_clients";
            this.textBox_clients.ReadOnly = true;
            this.textBox_clients.Size = new System.Drawing.Size(164, 21);
            this.textBox_clients.TabIndex = 0;
            this.textBox_clients.Text = "Players";
            // 
            // textBox_ip
            // 
            this.textBox_ip.BackColor = System.Drawing.Color.Gray;
            this.textBox_ip.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox_ip.Location = new System.Drawing.Point(116, 87);
            this.textBox_ip.Margin = new System.Windows.Forms.Padding(0);
            this.textBox_ip.Name = "textBox_ip";
            this.textBox_ip.ReadOnly = true;
            this.textBox_ip.Size = new System.Drawing.Size(164, 21);
            this.textBox_ip.TabIndex = 0;
            this.textBox_ip.Text = "IP";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.ForeColor = System.Drawing.Color.Gray;
            this.label6.Location = new System.Drawing.Point(15, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(82, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Server name";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.ForeColor = System.Drawing.Color.Gray;
            this.label7.Location = new System.Drawing.Point(15, 36);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(51, 13);
            this.label7.TabIndex = 1;
            this.label7.Text = "Up time";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.ForeColor = System.Drawing.Color.Gray;
            this.label8.Location = new System.Drawing.Point(15, 62);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(46, 13);
            this.label8.TabIndex = 1;
            this.label8.Text = "Clients";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.ForeColor = System.Drawing.Color.Gray;
            this.label9.Location = new System.Drawing.Point(15, 89);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(19, 13);
            this.label9.TabIndex = 1;
            this.label9.Text = "Ip";
            // 
            // tabPage_info
            // 
            this.tabPage_info.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.tabPage_info.Controls.Add(this.label9);
            this.tabPage_info.Controls.Add(this.label8);
            this.tabPage_info.Controls.Add(this.label7);
            this.tabPage_info.Controls.Add(this.label6);
            this.tabPage_info.Controls.Add(this.textBox_ip);
            this.tabPage_info.Controls.Add(this.textBox_clients);
            this.tabPage_info.Controls.Add(this.textBox_time);
            this.tabPage_info.Controls.Add(this.textBox_name);
            this.tabPage_info.Font = new System.Drawing.Font("Verdana", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabPage_info.Location = new System.Drawing.Point(4, 22);
            this.tabPage_info.Margin = new System.Windows.Forms.Padding(0);
            this.tabPage_info.Name = "tabPage_info";
            this.tabPage_info.Size = new System.Drawing.Size(285, 204);
            this.tabPage_info.TabIndex = 1;
            this.tabPage_info.Text = "Info";
            // 
            // NoEdgeWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 230);
            this.Controls.Add(this.panel_clientArea);
            this.Controls.Add(this.panel_serverOptions);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "NoEdgeWindow";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "No Edge dedicated server";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormClosingEvent);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.NoEdgeWindow_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.NoEdgeWindow_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.NoEdgeWindow_MouseUp);
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).EndInit();
            this.panel_serverOptions.ResumeLayout(false);
            this.panel_serverOptions.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.timeLimit)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nrOfClients)).EndInit();
            this.panel_clientArea.ResumeLayout(false);
            this.tabControl_options.ResumeLayout(false);
            this.tabPage_consol.ResumeLayout(false);
            this.panel_consol_interactive.ResumeLayout(false);
            this.panel_consol_interactive.PerformLayout();
            this.tabPage_info.ResumeLayout(false);
            this.tabPage_info.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox serverName;
        private System.Windows.Forms.Label label_serverName;
        private System.Windows.Forms.NumericUpDown listenPort;
        private System.Windows.Forms.Label label_listenPort;
        private System.Windows.Forms.Panel panel_serverOptions;
        private System.Windows.Forms.Panel panel_clientArea;
        private System.Windows.Forms.RichTextBox ServerInfoTextArea;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nrOfClients;
        private System.Windows.Forms.ComboBox gameModes;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown timeLimit;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox forceStart;
        private System.Windows.Forms.ComboBox mapName;
        private System.Windows.Forms.TabControl tabControl_options;
        private System.Windows.Forms.TabPage tabPage_consol;
        private System.Windows.Forms.Panel panel_consol_interactive;
        private System.Windows.Forms.Button button_consol_excexute;
        private System.Windows.Forms.TextBox textBox_consonl_textfield;
        private System.Windows.Forms.TabPage tabPage_info;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_ip;
        private System.Windows.Forms.TextBox textBox_clients;
        private System.Windows.Forms.TextBox textBox_time;
        private System.Windows.Forms.TextBox textBox_name;
    }
}

