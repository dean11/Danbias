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
            this.mapName = new System.Windows.Forms.TextBox();
            this.timeLimit = new System.Windows.Forms.NumericUpDown();
            this.gameModes = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.forceStart = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.nrOfClients = new System.Windows.Forms.NumericUpDown();
            this.buttonStartGame = new System.Windows.Forms.Button();
            this.panel_clientArea = new System.Windows.Forms.Panel();
            this.ServerInfoTextArea = new System.Windows.Forms.RichTextBox();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.clientInfoBox = new System.Windows.Forms.ListBox();
            this.labelClientsConnected = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).BeginInit();
            this.panel_serverOptions.SuspendLayout();
            this.panel_commands.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.timeLimit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nrOfClients)).BeginInit();
            this.panel_clientArea.SuspendLayout();
            this.SuspendLayout();
            // 
            // serverToggle
            // 
            this.serverToggle.Location = new System.Drawing.Point(9, 106);
            this.serverToggle.Name = "serverToggle";
            this.serverToggle.Size = new System.Drawing.Size(75, 23);
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
            this.panel_serverOptions.Location = new System.Drawing.Point(12, 12);
            this.panel_serverOptions.Name = "panel_serverOptions";
            this.panel_serverOptions.Size = new System.Drawing.Size(183, 141);
            this.panel_serverOptions.TabIndex = 6;
            // 
            // panel_commands
            // 
            this.panel_commands.Controls.Add(this.mapName);
            this.panel_commands.Controls.Add(this.timeLimit);
            this.panel_commands.Controls.Add(this.gameModes);
            this.panel_commands.Controls.Add(this.label3);
            this.panel_commands.Controls.Add(this.forceStart);
            this.panel_commands.Controls.Add(this.label2);
            this.panel_commands.Controls.Add(this.label4);
            this.panel_commands.Controls.Add(this.labelClientsConnected);
            this.panel_commands.Controls.Add(this.label1);
            this.panel_commands.Controls.Add(this.nrOfClients);
            this.panel_commands.Controls.Add(this.buttonStartGame);
            this.panel_commands.Location = new System.Drawing.Point(12, 159);
            this.panel_commands.Name = "panel_commands";
            this.panel_commands.Size = new System.Drawing.Size(183, 202);
            this.panel_commands.TabIndex = 7;
            this.panel_commands.Visible = false;
            // 
            // mapName
            // 
            this.mapName.Location = new System.Drawing.Point(78, 7);
            this.mapName.Name = "mapName";
            this.mapName.Size = new System.Drawing.Size(98, 20);
            this.mapName.TabIndex = 12;
            this.mapName.Text = "Unknown";
            // 
            // timeLimit
            // 
            this.timeLimit.Location = new System.Drawing.Point(109, 94);
            this.timeLimit.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.timeLimit.Name = "timeLimit";
            this.timeLimit.Size = new System.Drawing.Size(68, 20);
            this.timeLimit.TabIndex = 11;
            this.timeLimit.ThousandsSeparator = true;
            this.timeLimit.Value = new decimal(new int[] {
            15,
            0,
            0,
            0});
            // 
            // gameModes
            // 
            this.gameModes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gameModes.FormattingEnabled = true;
            this.gameModes.Items.AddRange(new object[] {
            "Free-for-all",
            "Team death-match"});
            this.gameModes.Location = new System.Drawing.Point(78, 66);
            this.gameModes.Name = "gameModes";
            this.gameModes.Size = new System.Drawing.Size(99, 21);
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
            this.nrOfClients.Location = new System.Drawing.Point(78, 36);
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
            this.nrOfClients.Size = new System.Drawing.Size(39, 20);
            this.nrOfClients.TabIndex = 7;
            this.nrOfClients.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // buttonStartGame
            // 
            this.buttonStartGame.Location = new System.Drawing.Point(9, 176);
            this.buttonStartGame.Name = "buttonStartGame";
            this.buttonStartGame.Size = new System.Drawing.Size(75, 23);
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
            this.panel_clientArea.Location = new System.Drawing.Point(202, 12);
            this.panel_clientArea.Name = "panel_clientArea";
            this.panel_clientArea.Size = new System.Drawing.Size(303, 349);
            this.panel_clientArea.TabIndex = 8;
            // 
            // ServerInfoTextArea
            // 
            this.ServerInfoTextArea.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ServerInfoTextArea.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ServerInfoTextArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ServerInfoTextArea.Font = new System.Drawing.Font("GulimChe", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ServerInfoTextArea.ForeColor = System.Drawing.SystemColors.Info;
            this.ServerInfoTextArea.Location = new System.Drawing.Point(0, 152);
            this.ServerInfoTextArea.Name = "ServerInfoTextArea";
            this.ServerInfoTextArea.ReadOnly = true;
            this.ServerInfoTextArea.Size = new System.Drawing.Size(303, 197);
            this.ServerInfoTextArea.TabIndex = 1;
            this.ServerInfoTextArea.Text = "";
            // 
            // splitter1
            // 
            this.splitter1.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitter1.Location = new System.Drawing.Point(0, 147);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(303, 5);
            this.splitter1.TabIndex = 2;
            this.splitter1.TabStop = false;
            // 
            // clientInfoBox
            // 
            this.clientInfoBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.clientInfoBox.FormattingEnabled = true;
            this.clientInfoBox.Location = new System.Drawing.Point(0, 0);
            this.clientInfoBox.Name = "clientInfoBox";
            this.clientInfoBox.Size = new System.Drawing.Size(303, 147);
            this.clientInfoBox.TabIndex = 0;
            // 
            // labelClientsConnected
            // 
            this.labelClientsConnected.AutoSize = true;
            this.labelClientsConnected.Location = new System.Drawing.Point(9, 149);
            this.labelClientsConnected.Name = "labelClientsConnected";
            this.labelClientsConnected.Size = new System.Drawing.Size(104, 13);
            this.labelClientsConnected.TabIndex = 8;
            this.labelClientsConnected.Text = "Clients connected: 0";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(517, 373);
            this.Controls.Add(this.panel_clientArea);
            this.Controls.Add(this.panel_commands);
            this.Controls.Add(this.panel_serverOptions);
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
        private System.Windows.Forms.TextBox mapName;
        private System.Windows.Forms.CheckBox forceStart;
        private System.Windows.Forms.Label labelClientsConnected;
    }
}

