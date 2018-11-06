#include "resources.h"
#include "brainmap.h"
#include "controllerstate.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFont>
#include <QtWidgets>

float BrainMap::vertices[] = {
	// positions           // texture coords
	1.0f, 1.0f, 0.0f,        1.0f, 1.0f, // top right
	1.0f, -1.0f, 0.0f,        1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 1.0f,       0.0f, 1.0f  // top left 
};
unsigned int BrainMap::indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

BrainMap::BrainMap(QWidget* parent)
	: QOpenGLWidget(parent)
{
	gui.container = new QWidget(this);
	gui.contLayout = new QVBoxLayout(gui.container);
	gui.r1Layout = new QHBoxLayout(gui.container);
	gui.r2Layout = new QHBoxLayout(gui.container);
	gui.r3Layout = new QHBoxLayout(gui.container);
	gui.r4Layout = new QHBoxLayout(gui.container);
	gui.r5Layout = new QHBoxLayout(gui.container);
	gui.box_filterDisp = new QComboBox(this);
	gui.box_filter1 = new QComboBox(this);
	gui.box_filter2 = new QComboBox(this);
	gui.btn_color = new QToolButton(gui.container);
	gui.slider_head = new QSlider(Qt::Orientation::Horizontal, gui.container);
	gui.slider_elec = new QSlider(Qt::Orientation::Horizontal, gui.container);
	
	gui.box_filterDisp->addItem("Sp. Filter 1");
	gui.box_filterDisp->addItem("Sp. Filter 2");
	gui.box_filter1->addItem("CAR");
	gui.box_filter1->addItem("Laplacian");
	gui.box_filter2->addItem("CAR");
	gui.box_filter2->addItem("Laplacian");
	gui.btn_color->setCheckable(true);
	gui.btn_color->setChecked(true);
	gui.btn_color->setText("Show Colours");
	gui.slider_head->setValue(50);
	gui.slider_elec->setValue(50);

	gui.container->setLayout(gui.contLayout);
	gui.r1Layout->addWidget(new QLabel("Display"));
	gui.r1Layout->addWidget(gui.box_filterDisp);
	gui.r1Layout->addWidget(gui.btn_color);
	gui.r2Layout->addWidget(new QLabel("Spatial Filter 1"));
	gui.r2Layout->addWidget(gui.box_filter1);
	gui.r3Layout->addWidget(new QLabel("Spatial Filter 2"));
	gui.r3Layout->addWidget(gui.box_filter2);
	gui.r4Layout->addWidget(new QLabel("Head Brightness"));
	gui.r4Layout->addWidget(gui.slider_head);
	gui.r5Layout->addWidget(new QLabel("Elec Brightness "));
	gui.r5Layout->addWidget(gui.slider_elec);

	gui.contLayout->addLayout(gui.r1Layout);
	gui.contLayout->addLayout(gui.r2Layout);
	gui.contLayout->addLayout(gui.r3Layout);
	gui.contLayout->addLayout(gui.r4Layout);
	gui.contLayout->addLayout(gui.r5Layout);
}

void BrainMap::initializeGL()
{
	try
	{
		ctx = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
		ctx->initializeOpenGLFunctions();
		ctx->glEnable(GL_BLEND);
		ctx->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// create + compile shaders ; add to shader program
		m_sprogram = new ShaderProgram(ctx);
		m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_brainmap.glsl"));
		m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_brainmap.glsl"));
		m_sprogram->compile();
		m_sprogram->setActive();

		ctx->glGenVertexArrays(1, &VAO);
		ctx->glGenBuffers(1, &VBO);
		ctx->glGenBuffers(1, &EBO);

		ctx->glBindVertexArray(VAO);
		{
			ctx->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			ctx->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
			ctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			ctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
			ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			ctx->glEnableVertexAttribArray(0);
			ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			ctx->glEnableVertexAttribArray(1);
		}
		ctx->glBindVertexArray(0);

		m_brainTex = new glw::Texture(ctx, "textures/head_10_20.png");
		m_activityTex = new glw::Texture(ctx, "textures/activity.png");

		m_textures = new TextureGroup(ctx);
		for (size_t i = 0; i < 16; ++i)
		{
			m_textures->addTexture("textures/ch" + std::to_string(i + 1) + ".png");
			m_sprogram->setTextureLocation("channels", *m_textures->getTexture(i));
		}

		m_sprogram->setTextureLocation("braintex", *m_brainTex);
		m_sprogram->setTextureLocation("activitytex", *m_activityTex);

		QTimer* timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		timer->start(33);
	}
	catch (DetailedException& e)
	{
		DEBUG_PRINTLNY(e.getMessage(), MSG_TYPE::FATAL);
	}
}

void BrainMap::resizeGL(int width, int height)
{
	width; height;
}

void BrainMap::exportState()
{
	std::lock_guard<std::mutex> lock(state->mtx_data);

	// export spatial filter parameters (for CAR/Laplacian)
	state->properties.spatial1Params = elecManager.elecStateLeft;
	state->properties.spatial2Params = elecManager.elecStateRight;
}

void BrainMap::paintGL()
{
	std::vector<double> tmp;
	{
		std::lock_guard<std::mutex> lock(ControllerState::state.mtx_data);
		tmp = ControllerState::state.chCAR_TR;
	}

	//sassert(tmp.size() <= 16);
	if (elecManager.getElectrodeCount() != tmp.size()) elecManager.setElectrodeCount(tmp.size());
	elecManager.chCAR_TR = tmp;

	{
		//QPainter painter(this);
		//painter.beginNativePainting();

		// vertex shader
		m_sprogram->setActive();
		int posLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "pos");
		int sizeLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "size");
		int aspectRatioLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "aspectRatio");
		// fragment shader
		int elecBrLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "elecBrightness");
		int headBrLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "headBrightness");
		int useTexLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "useTex");
		int magLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "mag");
		int selectLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "selected");
		int colLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "col");

		ctx->glClearColor(1.0, 1.0, 1.0, 1.0);

		m_brainTex->basic.glwActiveTexture();
		m_brainTex->basic.glwBindTexture();
		m_activityTex->basic.glwActiveTexture();
		m_activityTex->basic.glwBindTexture();

		// === constand for all textures ===
		ctx->glUniform1f(aspectRatioLoc, GLfloat(width()) / GLfloat(height()));
		ctx->glUniform1f(elecBrLoc, GLfloat(gui.slider_elec->value() / 99.0));
		ctx->glUniform1f(headBrLoc, GLfloat(gui.slider_head->value() / 99.0));

		// === draw background ===
		ctx->glUniform2f(posLoc, (GLfloat)0.0, (GLfloat)0.0);
		ctx->glUniform1f(sizeLoc, 1.0);
		ctx->glUniform1i(useTexLoc, (GLint)1);
		ctx->glUniform1f(magLoc, 1.0);
		
		ctx->glBindVertexArray(VAO);
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ctx->glBindVertexArray(0);

		// === draw shading around electrodes ===
		bool left = gui.box_filterDisp->currentIndex() == 0;
		bool showColor = gui.btn_color->isChecked();
		ctx->glBindVertexArray(VAO);
		for (int i = 0; i < elecManager.get().size(); ++i)
		{
			double x = elecManager.get()[i].pos.x;
			double y = elecManager.get()[i].pos.y;
			ctx->glUniform2f(posLoc, (GLfloat)(x), (GLfloat)(y));
			ctx->glUniform1f(sizeLoc, 0.5f);
			ctx->glUniform1i(useTexLoc, (GLint)0);
			int col = 2;
			auto tmp = elecManager.getColor(i,left);
			if (tmp > 0) col = 0;
			else if (tmp < 0) col = 1;
			ctx->glUniform1i(colLoc, (GLint)(col));
			if (showColor)
				ctx->glUniform1f(magLoc,(GLfloat)1.0);
			else
			{
				double mag = elecManager.chCAR_TR[i];
				mag = mag / 2 + 0.5;
				mag = std::clamp(mag, 0.0, 1.0);
				ctx->glUniform1f(magLoc, (GLfloat)mag);
			}
				
			ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// === draw channel names ===
		for (int i = 0; i < elecManager.get().size(); ++i)
		{
			if (i >= 16) break;
			m_textures->setActive(i);
			double x = elecManager.get()[i].pos.x;
			double y = elecManager.get()[i].pos.y;
			if (elecManager.isSelected(i))
			{
				ctx->glUniform1i(selectLoc, (GLint)0);
				ctx->glUniform1f(sizeLoc, 0.08f);
			}
			else
			{
				ctx->glUniform1f(selectLoc, (GLint)1);
				ctx->glUniform1f(sizeLoc, 0.07f);
			}
			ctx->glUniform2f(posLoc, (GLfloat)(x), (GLfloat)(y));
			
			ctx->glUniform1i(useTexLoc, (GLint)2);
			ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		ctx->glBindVertexArray(0);

		//painter.endNativePainting();
	}
	exportState();
	//drawLabels();
}

void BrainMap::drawLabels()
{
	QPainter painter(this);
	QFont font("Times", 8, QFont::Bold);
	painter.setPen(QColor(50, 150, 70));
	font.setPixelSize(8);
	painter.setFont(font);

	for (size_t i = 0; i < elecManager.get().size(); ++i)
	{
		int x = int(width()*0.5*(1.0 + elecManager.get()[i].pos.x));
		int y = int(height()*0.5*(1.0 + -elecManager.get()[i].pos.y));

		painter.drawText(x, y, "Ch " + QString::number(i));
	}
}

void BrainMap::mousePressEvent(QMouseEvent* event)
{
	if (elecManager.getElectrodeCount() == 0) return;

	float x = 2.0f * event->x() / (float)this->width() - 1.0f;
	float y = 1.0f - 2.0f * event->y() / (float)this->height();

	bool left = gui.box_filterDisp->currentIndex() == 0;
	elecManager.testSelect({ x,y });
	if (event->button() == Qt::MouseButton::RightButton)
		elecManager.switchColor(left);
}

void BrainMap::mouseReleaseEvent(QMouseEvent* event)
{
	if (elecManager.getElectrodeCount() == 0) return;

	event;
	elecManager.deselect();
}

void BrainMap::mouseMoveEvent(QMouseEvent* event)
{
	if (elecManager.getElectrodeCount() == 0) return;

	float x = 2.0f * event->x() / (float)this->width() - 1.0f;
	float y = 1.0f - 2.0f * event->y() / (float)this->height();

	elecManager.moveElectrode({ x,y });
}