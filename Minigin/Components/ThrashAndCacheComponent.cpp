#include "ThrashAndCacheComponent.h"

#include <chrono>
#include <corecrt_math.h>

#include <fstream>

#include "imgui.h"
#include "implot.h"

dae::ThrashAndCacheComponent::ThrashAndCacheComponent(GameObject& go)
	: Component(go)
{
}

void dae::ThrashAndCacheComponent::Update()
{

}

void dae::ThrashAndCacheComponent::ImGuiUpdate()
{
    ImVec2 ws = ImVec2(650, 500);
    ImVec2 ws2 = ImVec2(650, 550);


    // Window 1
    ImGui::Begin("Exercise 1");
    ImGui::SetWindowSize(ws);
    {
        ImGui::SeparatorText("Generate");
        ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);
        if (ImGui::Button("Exercise 1", sz))
        {
            Exercise1();
        }

        ImGui::SeparatorText("Plot");
        if (ImPlot::BeginPlot("Trash and cash - Int array"))
        {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit);
            //ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
            ImPlot::PlotLine("Ex 1", m_Ex1_x.data(), m_Ex1_y.data(), static_cast<int>(m_Ex1_x.size()));
            ImPlot::EndPlot();
        }
    }
    ImGui::End();

    // Window 2
    ImGui::Begin("Exercise 2");
    ImGui::SetWindowSize(ws);
    {
        ImGui::SeparatorText("Generate");
        ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);
        if (ImGui::Button("Exercise 2 - GameObject3D", sz))
        {
            Exercise2_Go();
        }
        if (ImGui::Button("Exercise 2 - GameObject3DAlt", sz))
        {
            Exercise2_Goa();
        }


        ImGui::SeparatorText("Plot");
        if (ImPlot::BeginPlot("Trash and cash - Int array"))
        {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Ex 2 - GO", m_Ex2_go_x.data(), m_Ex2_go_y.data(), static_cast<int>(m_Ex2_go_x.size()));
            ImPlot::PlotLine("Ex 2 - GOA", m_Ex2_goa_x.data(), m_Ex2_goa_y.data(), static_cast<int>(m_Ex2_goa_x.size()));
            ImPlot::EndPlot();
        }
    }
    ImGui::End();

    // Window 3
    ImGui::Begin("Trash and cache - Exercise 3");
    ImGui::SetWindowSize(ws2);
    {
	    ImGui::SeparatorText("Generate");
	    ImGui::InputInt("# Samples", &m_NrOfSamples);

	    ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);
	    if (ImGui::Button("Exercise 1", sz))
	    {
	        Exercise1();
	    }
	    if (ImGui::Button("Exercise 2 - GameObject3D", sz))
	    {
	        Exercise2_Go();
	    }
	    if (ImGui::Button("Exercise 2 - GameObject3DAlt", sz))
	    {
	        Exercise2_Goa();
	    }

	    ImGui::SeparatorText("Plot");
	    if (ImPlot::BeginPlot("Trash and Cache")) 
	    {
	        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels,ImPlotAxisFlags_AutoFit);
	                //ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
	        ImPlot::PlotLine("Ex 1", m_Ex1_x.data(), m_Ex1_y.data(), static_cast<int>(m_Ex1_x.size()));
	        ImPlot::PlotLine("Ex 2 - GO", m_Ex2_go_x.data(), m_Ex2_go_y.data(), static_cast<int>(m_Ex2_go_x.size()));
	        ImPlot::PlotLine("Ex 2 - GOA", m_Ex2_goa_x.data(), m_Ex2_goa_y.data(), static_cast<int>(m_Ex2_goa_x.size()));

    		ImPlot::EndPlot();
	    }
        ImGui::SeparatorText("Save to CSV");
        if (ImGui::Button("Save to CSV", sz))
        {
            SaveToCSV();
        }
    }
    ImGui::End();
}

void dae::ThrashAndCacheComponent::Exercise1()
{
    m_Ex1_x.clear();
    m_Ex1_y.clear();

    int* arr = new int[m_TESTSIZE];

    std::vector<std::vector<long long>> ex1_x{};
    std::vector<std::vector<long long>> ex1_y{};

    ex1_x.resize(m_NrOfSamples);
    ex1_y.resize(m_NrOfSamples);

    for (int smpl{}; smpl < m_NrOfSamples; ++smpl)
    {
        int testnr{ 1 };
	    for (int step{ 1 }; step <= 1024; step *= 2)
	    {
	        auto start = std::chrono::high_resolution_clock::now();

	        for (int index{}; index < m_TESTSIZE; index += step)
	        {
	            arr[index] *= 2;
	        }

	        auto end = std::chrono::high_resolution_clock::now();
	        auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            ex1_x[smpl].emplace_back(testnr);
            ex1_y[smpl].emplace_back(elapsedTime);
	        testnr++;
	    }
    }

    for (int index{}; index < ex1_x[0].size(); ++index)
    {
        long long result_x{};
        long long result_y{};
	    for (int sampl{}; sampl < m_NrOfSamples; ++sampl)
	    {
            result_x += ex1_x[sampl][index];
            result_y += ex1_y[sampl][index];
	    }
        result_x /= m_NrOfSamples;
        result_y /= m_NrOfSamples;

        m_Ex1_x.emplace_back(result_x);
        m_Ex1_y.emplace_back(result_y);
    }

    delete[] arr;
}

void dae::ThrashAndCacheComponent::Exercise2_Go()
{
    struct transform
    {
        float matrix[16] =
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    struct Gameobject3D
    {
        transform local{};
        int id;
    };

    Gameobject3D* arr = new Gameobject3D[m_TESTSIZE];

    m_Ex2_go_x.clear();
    m_Ex2_go_y.clear();
    std::vector<std::vector<long long>> ex2_x{};
    std::vector<std::vector<long long>> ex2_y{};

    ex2_x.resize(m_NrOfSamples);
    ex2_y.resize(m_NrOfSamples);

    for (int smpl{}; smpl < m_NrOfSamples; ++smpl)
    {
        int testnr{ 1 };
        for (int step{ 1 }; step <= 1024; step *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (int index{}; index < m_TESTSIZE; index += step)
            {
                arr[index].id += 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            ex2_x[smpl].emplace_back(testnr);
            ex2_y[smpl].emplace_back(elapsedTime);
            testnr++;
        }
    }

    for (int index{}; index < ex2_x[0].size(); ++index)
    {
        long long result_x{};
        long long result_y{};
        for (int sampl{}; sampl < m_NrOfSamples; ++sampl)
        {
            result_x += ex2_x[sampl][index];
            result_y += ex2_y[sampl][index];
        }
        result_x /= m_NrOfSamples;
        result_y /= m_NrOfSamples;

        m_Ex2_go_x.emplace_back(result_x);
        m_Ex2_go_y.emplace_back(result_y);
    }

    delete[] arr;
}

void dae::ThrashAndCacheComponent::Exercise2_Goa()
{
    struct transform
    {
        float matrix[16] =
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    struct Gameobject3DAlt
    {
        transform* local = new transform{};
        int id;
        ~Gameobject3DAlt() { delete local; }
    };

    Gameobject3DAlt* arr = new Gameobject3DAlt[m_TESTSIZE];

    m_Ex2_goa_x.clear();
    m_Ex2_goa_y.clear();
    std::vector<std::vector<long long>> ex2_x{};
    std::vector<std::vector<long long>> ex2_y{};

    ex2_x.resize(m_NrOfSamples);
    ex2_y.resize(m_NrOfSamples);

    for (int smpl{}; smpl < m_NrOfSamples; ++smpl)
    {
        int testnr{ 1 };
        for (int step{ 1 }; step <= 1024; step *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (int index{}; index < m_TESTSIZE; index += step)
            {
                arr[index].id += 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            ex2_x[smpl].emplace_back(testnr);
            ex2_y[smpl].emplace_back(elapsedTime);
            testnr++;
        }
    }

    for (int index{}; index < ex2_x[0].size(); ++index)
    {
        long long result_x{};
        long long result_y{};
        for (int sampl{}; sampl < m_NrOfSamples; ++sampl)
        {
            result_x += ex2_x[sampl][index];
            result_y += ex2_y[sampl][index];
        }
        result_x /= m_NrOfSamples;
        result_y /= m_NrOfSamples;

        m_Ex2_goa_x.emplace_back(result_x);
        m_Ex2_goa_y.emplace_back(result_y);
    }

    delete[] arr;
}

void dae::ThrashAndCacheComponent::SaveToCSV()
{
    std::ofstream file("output.csv");

    const char sep = ';';

    if (!file.is_open()) 
    {
        throw std::runtime_error("Error opening file for writing.");
    }

    // Sheet 1
    file << "Meta" << std::endl;
    file << "SampleRate" << sep << m_NrOfSamples << std::endl;
    file << "Container size" << sep << m_TESTSIZE << std::endl;
    file << std::endl;

    file << "Sheet 1" << std::endl;
    file << "Sample step" << sep << "Time_int" << std::endl;

    for (size_t i = 0; i < m_Ex1_y.size(); ++i) 
    {
        file << m_Ex1_x[i] << sep << m_Ex1_y[i] << std::endl;
    }

    // Sheet 2
    file << std::endl;

    file << "Meta" << std::endl;
    file << "SampleRate" << sep << m_NrOfSamples << std::endl;
    file << "Container size" << sep << m_TESTSIZE << std::endl;
    file << std::endl;

    file << "Sheet 2" << std::endl;
    file << "Sample step"<< sep << "Time_GameObject3D" << sep << "Time_GameObject3DAlt" << std::endl;

    for (size_t i = 0; i < m_Ex2_go_x.size(); ++i)
    {
        file << m_Ex2_go_x[i] << sep << m_Ex2_go_y[i] << sep << m_Ex2_goa_y[i] << std::endl;
    }

    file.close();
}
